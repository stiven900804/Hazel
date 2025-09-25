#include "hzpch.h"
#include "TmxSerializer.h"

#include "Hazel/Scene/Entity.h" 
#include <nlohmann/json.hpp>
#include <fstream>

namespace Hazel
{
	struct TilesetData
	{
		nlohmann::json data;

		TilesetData() = default;
		TilesetData(nlohmann::json&& j) : data(std::move(j)) {}

		// 可添加辅助方法
		nlohmann::json& GetJson() { return data; }
		const nlohmann::json& GetJson() const { return data; }
	};

	TmxSerializer::TmxSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
		m_MapMetadata = CreateRef<TmxMapMetadata>();
	}

	void TmxSerializer::Serialize(const std::string& filepath)
	{
		// 1. 加载 JSON 文件
		auto path = std::filesystem::path(filepath);
		std::ifstream file(path);
		if (!file.is_open()) 
		{
			HZ_CORE_ERROR("无法打开关卡文件: {}", filepath);
			return ;
		}

		// 2. 解析 JSON 数据
		nlohmann::json json_data;
		try {
			file >> json_data;
		}
		catch (const nlohmann::json::parse_error& e) {
			HZ_CORE_ERROR("解析 JSON 数据失败: {}", e.what());
			return ;
		}

	}

	void TmxSerializer::SerializeRuntime(const std::string& filepath)
	{
		// TODO: 实现运行时序列化（如只保存必要数据）
		Serialize(filepath);
	}

	bool TmxSerializer::Deserialize(const std::string& filepath)
	{
		// 1. 加载 JSON 文件
		auto path = std::filesystem::path(filepath);
		std::ifstream file(path);
		if (!file.is_open()) {
			HZ_CORE_ERROR("无法打开关卡文件: {}", filepath);
			return false;
		}

		// 2. 解析 JSON 数据
		nlohmann::json json_data;
		try 
		{
			file >> json_data;
		}
		catch (const nlohmann::json::parse_error& e) {
			HZ_CORE_ERROR("解析 JSON 数据失败: {}", e.what());
			return false;
		}

		// 3. 获取基本地图信息 (名称、地图尺寸、瓦片尺寸)
		m_MapMetadata->MapPath = filepath;
		m_MapMetadata->MapSize = glm::ivec2(json_data.value("width", 0), json_data.value("height", 0));
		m_MapMetadata->TileSize = glm::ivec2(json_data.value("tilewidth", 0), json_data.value("tileheight", 0));

		// 4. 加载 tileset 数据
		if (json_data.contains("tilesets") && json_data["tilesets"].is_array()) {
			for (const auto& tileset_json : json_data["tilesets"]) {
				if (!tileset_json.contains("source") || !tileset_json["source"].is_string() ||
					!tileset_json.contains("firstgid") || !tileset_json["firstgid"].is_number_integer()) {
					HZ_CORE_ERROR("tilesets 对象中缺少有效 'source' 或 'firstgid' 字段。");
					continue;
				}
				auto tileset_path = resolvePath(tileset_json["source"].get<std::string>(), m_MapMetadata->MapPath);  // 支持隐式转换，可以省略.get<T>()方法，
				auto first_gid = tileset_json["firstgid"];
				loadTileset(tileset_path, first_gid);
			}
		}

		// 5. 加载图层数据
		if (!json_data.contains("layers") || !json_data["layers"].is_array()) {       // 地图文件中必须有 layers 数组
			HZ_CORE_ERROR("地图文件 '{}' 中缺少或无效的 'layers' 数组。", m_MapMetadata->MapPath);
			return false;
		}
		for (const auto& layer_json : json_data["layers"]) {
			// 获取各图层对象中的类型（type）字段
			std::string layer_type = layer_json.value("type", "none");
			if (!layer_json.value("visible", true)) {
				HZ_CORE_INFO("图层 '{}' 不可见，跳过加载。", layer_json.value("name", "Unnamed"));
				continue;
			}
			;
			// 根据图层类型决定加载方法
			if (layer_type == "imagelayer") 
			{
                loadImageLayer(TilesetData(nlohmann::json(layer_json)), m_Scene);
			}
			else if (layer_type == "tilelayer") {
				//loadTileLayer(layer_json, scene);
			}
			else if (layer_type == "objectgroup") {
				//loadObjectLayer(layer_json, scene);
			}
			else {
				HZ_CORE_WARN("不支持的图层类型: {}", m_MapMetadata->MapPath);
			}
		}

		spdlog::info("关卡加载完成: {}", m_MapMetadata->MapPath);
		return true;
	}

	bool TmxSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// TODO: 实现运行时反序列化（如只加载必要数据）
		return Deserialize(filepath);
	}

	std::string TmxSerializer::resolvePath(std::string_view relative_path, std::string_view file_path)
	{
		try 
		{
			// 获取地图文件的父目录（相对于可执行文件） "assets/maps/level1.tmj" -> "assets/maps"
			auto map_dir = std::filesystem::path(file_path).parent_path();
			// 合并路径（相对于可执行文件）并返回。 /* std::filesystem::canonical：解析路径中的当前目录（.）和上级目录（..）导航符，
											  /*  得到一个干净的路径 */
			auto final_path = std::filesystem::canonical(map_dir / relative_path);
			return final_path.string();
		}
		catch (const std::exception& e) 
		{
			HZ_CORE_ERROR("解析路径失败: {}", e.what());
			return std::string(relative_path);
		}
	}


	void TmxSerializer::loadTileset(std::string_view tileset_path, int first_gid)
	{
		auto path = std::filesystem::path(tileset_path);
		std::ifstream tileset_file(path);
		if (!tileset_file.is_open()) {
			HZ_CORE_ERROR("无法打开 Tileset 文件: {}", tileset_path);
			return;
		}

		nlohmann::json ts_json;

		try 
		{
			tileset_file >> ts_json;
		}
		catch (const nlohmann::json::parse_error& e) 
		{
			HZ_CORE_ERROR("解析 Tileset JSON 文件 '{}' 失败: {} (at byte {})", tileset_path, e.what(), e.byte);
			return;
		}
		ts_json["file_path"] = tileset_path;    // 将文件路径存储到json中，后续解析图片路径时需要
		m_MapMetadata->TilesetDatas[first_gid] = TilesetData(std::move(ts_json));
		HZ_CORE_INFO("Tileset 文件 '{}' 加载完成，firstgid: {}", tileset_path, first_gid);
	}

    void TmxSerializer::loadImageLayer(const TilesetData& layerData, Ref<Scene>& scene)
    {
		// 1. 获取图层名称
		const auto& layer_json = layerData.GetJson();
		std::string layer_name = layer_json.value("name", "Unnamed");

		// 2. 获取图片路径
		if (!layer_json.contains("image") || !layer_json["image"].is_string()) {
			HZ_CORE_WARN("ImageLayer '{}' 缺少 'image' 字段，跳过。", layer_name);
			return;
		}
		std::string image_path = layer_json["image"];

		// 3. 解析图片路径（相对于地图文件）
		std::string resolved_image_path = resolvePath(image_path, m_MapMetadata->MapPath);

		// 4. 获取可选的偏移量
		float offset_x = layer_json.value("offsetx", 0.0f);
		float offset_y = layer_json.value("offsety", 0.0f);
		float imagewidth = layer_json.value("imagewidth", 1.0f);
		float imageheight = layer_json.value("imageheight", 1.0f);

		// 5. 获取视差因子及重复标志
		const glm::vec2 scroll_factor = glm::vec2(layer_json.value("parallaxx", 1.0f), layer_json.value("parallaxy", 1.0f));
		const glm::bvec2 repeat = glm::bvec2(layer_json.value("repeatx", false), layer_json.value("repeaty", false));

		// 6. 创建实体并添加相关组件
		Entity imageEntity = scene->CreateEntity(layer_name);

		// 添加 TransformComponent
		if (imageEntity.HasComponent<TransformComponent>()) {
			auto& transform = imageEntity.GetComponent<TransformComponent>();
			transform.Scale = glm::vec3(imagewidth ,imageheight, 1.0f);
			transform.Translation = glm::vec3(offset_x, offset_y, 0.0f);
		} else {
			TransformComponent transform;
			transform.Translation = glm::vec3(offset_x, offset_y, 0.0f);
			transform.Scale = glm::vec3(imagewidth, imageheight, 1.0f);
			imageEntity.AddComponent<TransformComponent>(transform);
		}

		// 添加 SpriteRendererComponent
		if (imageEntity.HasComponent<SpriteRendererComponent>()) {
			auto& sprite = imageEntity.GetComponent<SpriteRendererComponent>();
			sprite.Texture = Texture2D::Create(resolved_image_path);
			sprite.ScrollFactor = scroll_factor;
			sprite.Repeat = repeat;
		} else {
			SpriteRendererComponent sprite;
			sprite.Texture = Texture2D::Create(resolved_image_path);
			sprite.ScrollFactor = scroll_factor;
			sprite.Repeat = repeat;
			imageEntity.AddComponent<SpriteRendererComponent>(sprite);
		}
		HZ_CORE_INFO("ImageLayer '{}' 加载完成，图片路径: {}", layer_name, resolved_image_path);
    }



    
}

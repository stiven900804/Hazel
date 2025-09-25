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

		// ����Ӹ�������
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
		// 1. ���� JSON �ļ�
		auto path = std::filesystem::path(filepath);
		std::ifstream file(path);
		if (!file.is_open()) 
		{
			HZ_CORE_ERROR("�޷��򿪹ؿ��ļ�: {}", filepath);
			return ;
		}

		// 2. ���� JSON ����
		nlohmann::json json_data;
		try {
			file >> json_data;
		}
		catch (const nlohmann::json::parse_error& e) {
			HZ_CORE_ERROR("���� JSON ����ʧ��: {}", e.what());
			return ;
		}

	}

	void TmxSerializer::SerializeRuntime(const std::string& filepath)
	{
		// TODO: ʵ������ʱ���л�����ֻ�����Ҫ���ݣ�
		Serialize(filepath);
	}

	bool TmxSerializer::Deserialize(const std::string& filepath)
	{
		// 1. ���� JSON �ļ�
		auto path = std::filesystem::path(filepath);
		std::ifstream file(path);
		if (!file.is_open()) {
			HZ_CORE_ERROR("�޷��򿪹ؿ��ļ�: {}", filepath);
			return false;
		}

		// 2. ���� JSON ����
		nlohmann::json json_data;
		try 
		{
			file >> json_data;
		}
		catch (const nlohmann::json::parse_error& e) {
			HZ_CORE_ERROR("���� JSON ����ʧ��: {}", e.what());
			return false;
		}

		// 3. ��ȡ������ͼ��Ϣ (���ơ���ͼ�ߴ硢��Ƭ�ߴ�)
		m_MapMetadata->MapPath = filepath;
		m_MapMetadata->MapSize = glm::ivec2(json_data.value("width", 0), json_data.value("height", 0));
		m_MapMetadata->TileSize = glm::ivec2(json_data.value("tilewidth", 0), json_data.value("tileheight", 0));

		// 4. ���� tileset ����
		if (json_data.contains("tilesets") && json_data["tilesets"].is_array()) {
			for (const auto& tileset_json : json_data["tilesets"]) {
				if (!tileset_json.contains("source") || !tileset_json["source"].is_string() ||
					!tileset_json.contains("firstgid") || !tileset_json["firstgid"].is_number_integer()) {
					HZ_CORE_ERROR("tilesets ������ȱ����Ч 'source' �� 'firstgid' �ֶΡ�");
					continue;
				}
				auto tileset_path = resolvePath(tileset_json["source"].get<std::string>(), m_MapMetadata->MapPath);  // ֧����ʽת��������ʡ��.get<T>()������
				auto first_gid = tileset_json["firstgid"];
				loadTileset(tileset_path, first_gid);
			}
		}

		// 5. ����ͼ������
		if (!json_data.contains("layers") || !json_data["layers"].is_array()) {       // ��ͼ�ļ��б����� layers ����
			HZ_CORE_ERROR("��ͼ�ļ� '{}' ��ȱ�ٻ���Ч�� 'layers' ���顣", m_MapMetadata->MapPath);
			return false;
		}
		for (const auto& layer_json : json_data["layers"]) {
			// ��ȡ��ͼ������е����ͣ�type���ֶ�
			std::string layer_type = layer_json.value("type", "none");
			if (!layer_json.value("visible", true)) {
				HZ_CORE_INFO("ͼ�� '{}' ���ɼ����������ء�", layer_json.value("name", "Unnamed"));
				continue;
			}
			;
			// ����ͼ�����;������ط���
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
				HZ_CORE_WARN("��֧�ֵ�ͼ������: {}", m_MapMetadata->MapPath);
			}
		}

		spdlog::info("�ؿ��������: {}", m_MapMetadata->MapPath);
		return true;
	}

	bool TmxSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// TODO: ʵ������ʱ�����л�����ֻ���ر�Ҫ���ݣ�
		return Deserialize(filepath);
	}

	std::string TmxSerializer::resolvePath(std::string_view relative_path, std::string_view file_path)
	{
		try 
		{
			// ��ȡ��ͼ�ļ��ĸ�Ŀ¼������ڿ�ִ���ļ��� "assets/maps/level1.tmj" -> "assets/maps"
			auto map_dir = std::filesystem::path(file_path).parent_path();
			// �ϲ�·��������ڿ�ִ���ļ��������ء� /* std::filesystem::canonical������·���еĵ�ǰĿ¼��.�����ϼ�Ŀ¼��..����������
											  /*  �õ�һ���ɾ���·�� */
			auto final_path = std::filesystem::canonical(map_dir / relative_path);
			return final_path.string();
		}
		catch (const std::exception& e) 
		{
			HZ_CORE_ERROR("����·��ʧ��: {}", e.what());
			return std::string(relative_path);
		}
	}


	void TmxSerializer::loadTileset(std::string_view tileset_path, int first_gid)
	{
		auto path = std::filesystem::path(tileset_path);
		std::ifstream tileset_file(path);
		if (!tileset_file.is_open()) {
			HZ_CORE_ERROR("�޷��� Tileset �ļ�: {}", tileset_path);
			return;
		}

		nlohmann::json ts_json;

		try 
		{
			tileset_file >> ts_json;
		}
		catch (const nlohmann::json::parse_error& e) 
		{
			HZ_CORE_ERROR("���� Tileset JSON �ļ� '{}' ʧ��: {} (at byte {})", tileset_path, e.what(), e.byte);
			return;
		}
		ts_json["file_path"] = tileset_path;    // ���ļ�·���洢��json�У���������ͼƬ·��ʱ��Ҫ
		m_MapMetadata->TilesetDatas[first_gid] = TilesetData(std::move(ts_json));
		HZ_CORE_INFO("Tileset �ļ� '{}' ������ɣ�firstgid: {}", tileset_path, first_gid);
	}

    void TmxSerializer::loadImageLayer(const TilesetData& layerData, Ref<Scene>& scene)
    {
		// 1. ��ȡͼ������
		const auto& layer_json = layerData.GetJson();
		std::string layer_name = layer_json.value("name", "Unnamed");

		// 2. ��ȡͼƬ·��
		if (!layer_json.contains("image") || !layer_json["image"].is_string()) {
			HZ_CORE_WARN("ImageLayer '{}' ȱ�� 'image' �ֶΣ�������", layer_name);
			return;
		}
		std::string image_path = layer_json["image"];

		// 3. ����ͼƬ·��������ڵ�ͼ�ļ���
		std::string resolved_image_path = resolvePath(image_path, m_MapMetadata->MapPath);

		// 4. ��ȡ��ѡ��ƫ����
		float offset_x = layer_json.value("offsetx", 0.0f);
		float offset_y = layer_json.value("offsety", 0.0f);
		float imagewidth = layer_json.value("imagewidth", 1.0f);
		float imageheight = layer_json.value("imageheight", 1.0f);

		// 5. ��ȡ�Ӳ����Ӽ��ظ���־
		const glm::vec2 scroll_factor = glm::vec2(layer_json.value("parallaxx", 1.0f), layer_json.value("parallaxy", 1.0f));
		const glm::bvec2 repeat = glm::bvec2(layer_json.value("repeatx", false), layer_json.value("repeaty", false));

		// 6. ����ʵ�岢���������
		Entity imageEntity = scene->CreateEntity(layer_name);

		// ��� TransformComponent
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

		// ��� SpriteRendererComponent
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
		HZ_CORE_INFO("ImageLayer '{}' ������ɣ�ͼƬ·��: {}", layer_name, resolved_image_path);
    }



    
}

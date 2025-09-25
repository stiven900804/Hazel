#pragma once
#include "../Scene.h"

#include <map>

namespace Hazel
{
	struct TilesetData; // 只声明，不暴露 json

	struct TmxMapMetadata
	{
		std::string MapPath;
		glm::ivec2 MapSize;
		glm::ivec2 TileSize;
		std::map<int, TilesetData> TilesetDatas; // 用自定义类型代替 json
	};

	class TmxSerializer
	{
	public:
		TmxSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		std::string resolvePath(std::string_view relative_path, std::string_view file_path);
		void loadTileset(std::string_view tileset_path, int first_gid);

		void loadImageLayer(const TilesetData& layerData, Ref<Scene>& scene);
	private:
		Ref<Scene> m_Scene;
		Ref<TmxMapMetadata> m_MapMetadata;
	};
}






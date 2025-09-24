#include "hzpch.h"
#include "TmxSerializer.h"
#include "nlohmann/json.hpp"
#include <fstream>

namespace Hazel
{
	TmxSerializer::TmxSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
	}

	void TmxSerializer::Serialize(const std::string& filepath)
	{
		// TODO: 实现将 m_Scene 序列化为 TMX 格式并保存到文件
		std::ofstream out(filepath);
		if (!out.is_open())
			return;

		// 示例：写入 TMX 文件头
		out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		out << "<map version=\"1.0\" orientation=\"orthogonal\">\n";
		// 这里应遍历 m_Scene 的实体和组件，生成 TMX 格式内容
		out << "</map>\n";
		out.close();
	}

	void TmxSerializer::SerializeRuntime(const std::string& filepath)
	{
		// TODO: 实现运行时序列化（如只保存必要数据）
		Serialize(filepath);
	}

	bool TmxSerializer::Deserialize(const std::string& filepath)
	{
		// TODO: 实现从 TMX 文件反序列化到 m_Scene
		std::ifstream in(filepath);
		if (!in.is_open())
			return false;

		// 示例：读取 TMX 文件内容
		// 实际应解析 XML 并重建 Scene
		in.close();
		return true;
	}

	bool TmxSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// TODO: 实现运行时反序列化（如只加载必要数据）
		return Deserialize(filepath);
	}
}

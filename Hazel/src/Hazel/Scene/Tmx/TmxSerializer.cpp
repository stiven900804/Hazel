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
		// TODO: ʵ�ֽ� m_Scene ���л�Ϊ TMX ��ʽ�����浽�ļ�
		std::ofstream out(filepath);
		if (!out.is_open())
			return;

		// ʾ����д�� TMX �ļ�ͷ
		out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		out << "<map version=\"1.0\" orientation=\"orthogonal\">\n";
		// ����Ӧ���� m_Scene ��ʵ������������ TMX ��ʽ����
		out << "</map>\n";
		out.close();
	}

	void TmxSerializer::SerializeRuntime(const std::string& filepath)
	{
		// TODO: ʵ������ʱ���л�����ֻ�����Ҫ���ݣ�
		Serialize(filepath);
	}

	bool TmxSerializer::Deserialize(const std::string& filepath)
	{
		// TODO: ʵ�ִ� TMX �ļ������л��� m_Scene
		std::ifstream in(filepath);
		if (!in.is_open())
			return false;

		// ʾ������ȡ TMX �ļ�����
		// ʵ��Ӧ���� XML ���ؽ� Scene
		in.close();
		return true;
	}

	bool TmxSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// TODO: ʵ������ʱ�����л�����ֻ���ر�Ҫ���ݣ�
		return Deserialize(filepath);
	}
}

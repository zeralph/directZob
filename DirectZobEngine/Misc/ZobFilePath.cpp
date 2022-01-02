#include "ZobFilePath.h"
#include "../SceneLoader.h"

ZobFilePath::ZobFilePath()
{
	m_fileType = eFileType_all;
	Reset();
}

ZobFilePath::ZobFilePath(std::string name, std::string path, std::string file, bool bAbsolute)
{
	m_name = name;
	m_path = path;
	m_file = file;
	m_bAbsolute = bAbsolute;
	if (m_path.length() && m_path[m_path.length()- 1] != '/')
	{
		m_path.append("/");
	}
}

ZobFilePath::~ZobFilePath()
{

}

bool ZobFilePath::IsDefined()
{
	return m_name.size() && m_path.size() && m_file.size();
}

void ZobFilePath::Reset()
{
	m_name = "";
	m_path = ""; 
	m_file = "";
	m_bAbsolute = false;
}

std::string ZobFilePath::GetFullPath()
{
	std::string s;
	if (m_bAbsolute)
	{
		s = m_path + m_file;
	}
	else
	{
		s = std::string(SceneLoader::GetResourcePath());
		if (s.length() && s[s.length() - 1] != '/')
		{
			s .append("/");
		}
		s.append(m_path);
		s.append(m_file);
	}
	return s;
}

std::string ZobFilePath::GetFullPathWithoutFile()
{
	std::string s;
	if (m_bAbsolute)
	{
		s = m_path;
	}
	else
	{
		s = std::string(SceneLoader::GetResourcePath());
		s = "";
		s.append(m_path);
	}
	return s;
}

std::string ZobFilePath::Serialize()
{
	std::string s = m_name;
	std::string rsc = SceneLoader::GetResourcePath();
	std::size_t i = m_path.find(rsc);
	if (m_bAbsolute && i == 0)
	{
		m_bAbsolute = false;
		i = rsc.length();
		m_path = m_path.substr(i, m_path.length()-i-1);
		if (m_path.length() == 0 || m_path[m_path.length()-1] != '/')
		{
			m_path.append(std::string("/"));
		}
	}
	s = s.append(";").append(m_path).append(";").append(m_file).append(";").append(m_bAbsolute?"1":"0");
	return s;
}

void ZobFilePath::Unserialize(std::string s)
{
	std::size_t del1, del2, del3 = 0;
	del1 = s.find(';');
	if (del1 != std::string::npos)
	{
		m_name = s.substr(0, del1);
		del2 = s.find(';', del1 + 1);
		if (del1 != std::string::npos)
		{
			m_path = s.substr(del1 + 1, del2 - (del1 + 1));
			del3 = s.find(';', del2 + 1);
			if (del1 != std::string::npos)
			{
				m_file = s.substr(del2 + 1, del3 - (del2 + 1));
				m_bAbsolute = s.substr(del3 + 1, s.size() - 1) == "1";
			}
		}
	}
}
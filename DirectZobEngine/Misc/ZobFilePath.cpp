#include "ZobFilePath.h"
#include "../DirectZob.h"
#include "../SceneLoader.h"
#include <filesystem>
#include <iostream>

ZobFilePath::ZobFilePath()
{
	m_fileType = eFileType_all;
	m_buf = NULL;
	m_bufLen = 0;
	m_freeBuffer = true;
	Reset();
}

ZobFilePath::ZobFilePath(std::string name, std::string path, std::string file, bool bAbsolute)
{
	m_name = name;
	m_path = path;
	m_file = file;
	m_buf = NULL;
	m_bufLen = 0;
	m_bAbsolute = bAbsolute;
	if (m_path.length() && m_path[m_path.length()- 1] != '/')
	{
		m_path.append("/");
	}
	m_freeBuffer = true;
}

ZobFilePath::ZobFilePath(std::string name, char* buffer, long len)
{
	m_name = name;
	m_path = "/";
	m_file = name;
	m_buf = buffer;
	m_bufLen = 0;
	m_bAbsolute = false;
	m_freeBuffer = false;
}

ZobFilePath::~ZobFilePath()
{
	Reset();
}

bool ZobFilePath::IsDefined()
{
	return m_name.size() && m_path.size() && m_file.size();
}

void ZobFilePath::Reset()
{
	UnloadData();
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

void ZobFilePath::LoadData()
{
	if (m_buf != NULL)
	{
		return;
	}
	if (SceneLoader::LoadFromArchive())
	{
		std::string fp = m_path + m_file;
		std::filesystem::path pp = std::filesystem::path(fp.c_str());
		std::filesystem::path p = std::filesystem::weakly_canonical(pp);
		fp = p.u8string();
		std::replace(fp.begin(), fp.end(), '\\', '/');
		if (PHYSFS_exists(fp.c_str()))
		{
			PHYSFS_file* m_phyFile = PHYSFS_openRead(fp.c_str());
			if (m_phyFile)
			{
				m_bufLen = PHYSFS_fileLength(m_phyFile);
				m_buf = (char*)malloc(sizeof(char) * (m_bufLen + 1));
				int length_read = PHYSFS_read(m_phyFile, m_buf, sizeof(char), PHYSFS_fileLength(m_phyFile));
				m_buf[m_bufLen] = '\0';
				PHYSFS_close(m_phyFile);
			}
			else
			{
				DirectZob::LogError("cannot load %s from archive", fp.c_str());
			}
		}
		else
		{
			DirectZob::LogError("cannot find %s in archive", fp.c_str());
		}
	}
	else
	{
		std::string f = GetFullPath();
		/*
		FILE* fp;
		fp = fopen(f.c_str(), "r");
		fseek(fp, 0, SEEK_END);
		m_bufLen = ftell(fp);
		rewind(fp);
		m_buf = (char*)malloc(sizeof(char) * (m_bufLen + 1));
		fread(m_buf, sizeof(unsigned char), m_bufLen, fp);
		m_buf[m_bufLen] = '\0';
		*/
		std::ifstream infile;
		infile.open(f, std::ios::in | std::ios::binary);
		if (infile.good())
		{
			infile.seekg(0, std::ios::end);
			std::streamsize size = infile.tellg();
			infile.seekg(0, std::ios::beg);
			m_buf = (char*)malloc(sizeof(char) * (size + 1));
			infile.read(m_buf, size);
			m_buf[size] = '\0';
			m_bufLen = size;
		}
		else
		{
			m_bufLen = 0;
			m_buf = NULL;
		}
	}
}

void ZobFilePath::UnloadData()
{
	if (m_buf && m_freeBuffer)
	{
		free(m_buf);
		m_buf = NULL;
		m_bufLen = 0;
	}
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
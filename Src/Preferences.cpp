/****************************************************************
BeebEm - BBC Micro and Master 128 Emulator
Copyright (C) 2007  Mike Wyatt

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public
License along with this program; if not, write to the Free
Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA  02110-1301, USA.
****************************************************************/

#include <windows.h>

#include "Preferences.h"
#include "StringUtils.h"

//-----------------------------------------------------------------------------

static const int MAX_PREFS_LINE_LEN = 1024;

// Token written to start of pref files
#define PREFS_TOKEN "*** BeebEm Preferences ***"

//-----------------------------------------------------------------------------

static int Hex2Int(int hex)
{
	if (hex >= '0' && hex <= '9')
	{
		return hex - '0';
	}
	else if (hex >= 'A' && hex <= 'F')
	{
		return hex - 'A' + 10;
	}
	else if (hex >= 'a' && hex <= 'f')
	{
		return hex - 'a' + 10;
	}

	return 0;
}

//-----------------------------------------------------------------------------

Preferences::Result Preferences::Load(const char* FileName)
{
	FILE *fd = fopen(FileName, "r");

	if (fd == nullptr)
	{
		return Result::Failed;
	}

	char buf[MAX_PREFS_LINE_LEN];

	if (fgets(buf, MAX_PREFS_LINE_LEN - 1, fd) != nullptr)
	{
		if (strcmp(buf, PREFS_TOKEN "\n") != 0)
		{
			fclose(fd);
			return Result::InvalidFormat;
		}
		else
		{
			while (fgets(buf, MAX_PREFS_LINE_LEN - 1, fd) != nullptr)
			{
				char *val = strchr(buf, '=');

				if (val)
				{
					*val = 0;
					++val;
					if (val[strlen(val) - 1] == '\n')
						val[strlen(val) - 1] = 0;
					m_Prefs[buf] = val;
				}
			}
		}

		fclose(fd);
	}

	return Result::Success;
}

//-----------------------------------------------------------------------------

Preferences::Result Preferences::Save(const char* FileName)
{
	// Write the file
	FILE *fd = fopen(FileName, "w");

	if (fd == nullptr)
	{
		return Result::Failed;
	}

	fprintf(fd, PREFS_TOKEN "\n\n");

	for (const auto& i : m_Prefs)
	{
		fprintf(fd, "%s=%s\n", i.first.c_str(), i.second.c_str());
	}

	fclose(fd);

	return Result::Success;
}

//-----------------------------------------------------------------------------

bool Preferences::GetBinaryValue(const char* id, void* Value, size_t Size) const
{
	bool found = true;

	PrefsMap::const_iterator i = m_Prefs.find(id);

	if (i != m_Prefs.end())
	{
		const std::string& Data = i->second;

		if (Data.size() == Size * 2)
		{
			unsigned char* BinValue = reinterpret_cast<unsigned char *>(Value);

			for (size_t b = 0; b < Size; ++b)
			{
				BinValue[b] = (unsigned char)((Hex2Int(Data[b * 2]) << 4) | Hex2Int(Data[b * 2 + 1]));
			}
		}
		else
		{
			found = false;
		}
	}
	else
	{
		found = false;
	}

	return found;
}

//-----------------------------------------------------------------------------

void Preferences::SetBinaryValue(const char* id, const void* Value, size_t Size)
{
	char hx[MAX_PREFS_LINE_LEN];
	const unsigned char* BinValue = reinterpret_cast<const unsigned char *>(Value);

	for (size_t b = 0; b < Size; ++b)
	{
		sprintf(hx + b * 2, "%02x", static_cast<int>(BinValue[b]));
	}

	m_Prefs[id] = hx;
}

//-----------------------------------------------------------------------------

bool Preferences::GetStringValue(const char* id, char* Value) const
{
	bool found = true;

	PrefsMap::const_iterator i = m_Prefs.find(id);

	if (i != m_Prefs.end())
	{
		strcpy(Value, i->second.c_str());
	}
	else
	{
		found = false;
	}

	return found;
}

//-----------------------------------------------------------------------------

void Preferences::SetStringValue(const char* id, const char* Value)
{
	m_Prefs[id] = Value;
}

//-----------------------------------------------------------------------------

bool Preferences::GetStringValue(const char* id, std::string& Value) const
{
	bool found = true;

	PrefsMap::const_iterator i = m_Prefs.find(id);

	if (i != m_Prefs.end())
	{
		Value = i->second;
	}
	else
	{
		found = false;
	}

	return found;
}

//-----------------------------------------------------------------------------

void Preferences::SetStringValue(const char* id, const std::string& Value)
{
	m_Prefs[id] = Value;
}

//-----------------------------------------------------------------------------

bool Preferences::GetDWORDValue(const char* id, DWORD& Value) const
{
	bool found = true;

	PrefsMap::const_iterator i = m_Prefs.find(id);

	if (i != m_Prefs.end())
	{
		sscanf(i->second.c_str(), "%x", &Value);
	}
	else
	{
		found = false;
	}

	return found;
}

//-----------------------------------------------------------------------------

void Preferences::SetDWORDValue(const char* id, DWORD Value)
{
	char hx[MAX_PREFS_LINE_LEN];
	sprintf(hx, "%08x", Value);
	m_Prefs[id] = hx;
}

//-----------------------------------------------------------------------------

bool Preferences::GetDecimalValue(const char* id, int& Value) const
{
	PrefsMap::const_iterator i = m_Prefs.find(id);

	if (i == m_Prefs.end())
	{
		return false;
	}

	if (ParseNumber(i->second, &Value))
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------

bool Preferences::SetDecimalValue(const char* id, int Value)
{
	m_Prefs[id] = std::to_string(Value);

	return true;
}

//-----------------------------------------------------------------------------

bool Preferences::GetBoolValue(const char* id, bool& Value) const
{
	PrefsMap::const_iterator i = m_Prefs.find(id);

	if (i == m_Prefs.end())
	{
		return false;
	}

	try
	{
		Value = std::stoi(i->second) != 0;
	}
	catch (std::exception&)
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------

void Preferences::SetBoolValue(const char* id, bool Value)
{
	m_Prefs[id] = std::to_string(Value);
}

//-----------------------------------------------------------------------------

void Preferences::EraseValue(const char* id)
{
	m_Prefs.erase(id);
}

//-----------------------------------------------------------------------------

bool Preferences::HasValue(const char* id) const
{
	return m_Prefs.find(id) != m_Prefs.end();
}

//-----------------------------------------------------------------------------

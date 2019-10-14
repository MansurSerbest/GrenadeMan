#include "stdafx.h"
#include "SpriteFontLoader.h"
#include "../Helpers/BinaryReader.h"
#include "../Content/ContentManager.h"
#include "../Graphics/TextRenderer.h"
#include "../Graphics/TextureData.h"


SpriteFontLoader::SpriteFontLoader()
{

}


SpriteFontLoader::~SpriteFontLoader()
{
}

#pragma warning( disable : 4702 )
SpriteFont* SpriteFontLoader::LoadContent(const wstring& assetFile)
{
	auto pBinReader = new BinaryReader(); //Prevent memory leaks
	pBinReader->Open(assetFile);

	if (!pBinReader->Exists())
	{
		delete pBinReader;
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Failed to read the assetFile!\nPath: \'%s\'", assetFile.c_str());
		
		return nullptr;
	}

	//See BMFont Documentation for Binary Layout
	
	//Parse the Identification bytes (B,M,F)
	char B = pBinReader->Read<char>();
	char M = pBinReader->Read<char>();
	char F = pBinReader->Read<char>();
	//If Identification bytes doesn't match B|M|F,
	//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	//return nullptr
	if (B != 'B' || M != 'M' || F != 'F')
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font");
		return nullptr;
	}

	//Parse the version (version 3 required)
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	//...
	char version = pBinReader->Read<char>();
	if (version < 3)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported");
		return nullptr;
	}

	//Valid .fnt file
	auto pSpriteFont = new SpriteFont();
	//SpriteFontLoader is a friend class of SpriteFont
	//That means you have access to its privates (pSpriteFont->m_FontName = ... is valid)

	int blockID{};
	int blockSize{};

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve the FontSize (will be -25, BMF bug) [SpriteFont::m_FontSize]
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	//Retrieve the FontName [SpriteFont::m_FontName]
	
	blockID = pBinReader->Read<char>();
	blockSize = pBinReader->Read<int>();

	pSpriteFont->m_FontSize = pBinReader->Read<short>();
	pBinReader->MoveBufferPosition(12);
	pSpriteFont->m_FontName = pBinReader->ReadNullString();
	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve Texture Width & Height [SpriteFont::m_TextureWidth/m_TextureHeight]
	//Retrieve PageCount
	//> if pagecount > 1
	//> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed)
	//Advance to Block2 (Move Reader)
	blockID = pBinReader->Read<char>();
	blockSize = pBinReader->Read<int>();
	pBinReader->MoveBufferPosition(4);
	pSpriteFont->m_TextureWidth = pBinReader->Read<short>();
	pSpriteFont->m_TextureHeight = pBinReader->Read<short>();
	if(pBinReader->Read<short>() > 1)
		Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed");
	pBinReader->MoveBufferPosition(5);


	//**********
	// BLOCK 2 *
	//**********
	blockID = pBinReader->Read<char>();
	blockSize = pBinReader->Read<int>();
	//Retrieve the blockId and blockSize
	//Retrieve the PageName (store Local)
	//	> If PageName is empty
	//	> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty])
	//>Retrieve texture filepath from the assetFile path
	//> (ex. c:/Example/somefont.fnt => c:/Example/) [Have a look at: wstring::rfind()]
	//>Use path and PageName to load the texture using the ContentManager [SpriteFont::m_pTexture]
	//> (ex. c:/Example/ + 'PageName' => c:/Example/somefont_0.png)
	wstring pageName = pBinReader->ReadNullString();
	if (pageName.size() == 0)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty]");
	}
	wstring toFindString = pageName.substr(0, pageName.rfind(L'_'));
	size_t idx = assetFile.rfind(toFindString);
	wstring filePath = assetFile.substr(0, idx);
	pSpriteFont->m_pTexture = ContentManager::Load<TextureData>(filePath + pageName);
	
	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	blockID = pBinReader->Read<char>();
	blockSize = pBinReader->Read<int>();
	//Retrieve Character Count (see documentation)
	int characterCount = blockSize / 20;
	//Retrieve Every Character, For every Character:
	//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
	//> Check if CharacterId is valid (SpriteFont::IsCharValid), Log Warning and advance to next character if not valid
	//> Retrieve the corresponding FontMetric (SpriteFont::GetMetric) [REFERENCE!!!]
	//> Set IsValid to true [FontMetric::IsValid]
	//> Set Character (CharacterId) [FontMetric::Character]
	//> Retrieve Xposition (store Local)
	//> Retrieve Yposition (store Local)
	//> Retrieve & Set Width [FontMetric::Width]
	//> Retrieve & Set Height [FontMetric::Height]
	//> Retrieve & Set OffsetX [FontMetric::OffsetX]
	//> Retrieve & Set OffsetY [FontMetric::OffsetY]
	//> Retrieve & Set AdvanceX [FontMetric::AdvanceX]
	//> Retrieve & Set Page [FontMetric::Page]
	//> Retrieve Channel (BITFIELD!!!) 
	//	> See documentation for BitField meaning [FontMetrix::Channel]
	//> Calculate Texture Coordinates using Xposition, Yposition, TextureWidth & TextureHeight [FontMetric::TexCoord]
	for (int i{ 0 }; i <= characterCount-1; ++i)
	{
		int charID = pBinReader->Read<int>();
		wchar_t stringCharID = wchar_t(charID);
		if (pSpriteFont->IsCharValid(stringCharID) == false)
		{
			Logger::LogWarning(wstring(L"character is not valid!"));
			continue;
		}
		FontMetric &fontMetric = pSpriteFont->GetMetric(stringCharID);
		fontMetric.IsValid = true;
		fontMetric.Character = stringCharID;
		
		int posX = pBinReader->Read<short>();
		int posY = pBinReader->Read<short>();

		fontMetric.Width = pBinReader->Read<short>();
		fontMetric.Height = pBinReader->Read<short>();
		fontMetric.OffsetX = pBinReader->Read<short>();
		fontMetric.OffsetY = pBinReader->Read<short>();
		fontMetric.AdvanceX = pBinReader->Read<short>();
		fontMetric.Page = pBinReader->Read<UCHAR>();
		int channel = pBinReader->Read<UCHAR>();
		fontMetric.Channel = (channel == 1 ? 2 : (channel == 2 ? 1 : (channel == 4 ? 0 : 3)));

		float uvX = (float)posX / (float)pSpriteFont->m_TextureWidth;
		float uvY = (float)posY / (float)pSpriteFont->m_TextureHeight;
		fontMetric.TexCoord = XMFLOAT2{uvX, uvY};
	}
	
	//DONE :)

	delete pBinReader;
	return pSpriteFont;
	
	#pragma warning(default:4702)  
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
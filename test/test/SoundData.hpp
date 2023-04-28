/******************************************************************************
* 
* @file      SoundData.hpp
* @brief     サウンドデータ
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      データのファイル名列挙
* @attention 
* 
******************************************************************************/
#pragma once
LPCWSTR g_pszFileBGM[MAX_BGM] = {
	L"BGM001.mp3",		// ゲームBGM
	L"BGM001.mp3",			// タイトルBGM
};

LPCWSTR g_pszFileSE[MAX_SE] = {
	L"cancel000.wav",			// 弾発射音
	L"cancel000.wav",	// 爆発音
	L"cancel000.wav",			// 衝突音
	L"cancel000.wav",		// ゲームオーバーBGM
};

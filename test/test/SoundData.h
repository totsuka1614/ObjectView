/******************************************************************************
* 
* @file      SoundData.h
* @brief     サウンドデータ
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      サウンドを列挙する
* @attention 
* 
******************************************************************************/
#pragma once
enum eBGM {
	BGM_GAME = 0,	// ゲームBGM
	BGM_TITLE,		// タイトルBGM

	MAX_BGM
};

enum eSE {
	SE_SHOT = 0,	// 弾発射音
	SE_EXPLOSION,	// 爆発音
	SE_DEFEND,		// 衝突音
	SE_GAMEOVER,	// ゲームオーバーBGM

	MAX_SE
};

/******************************************************************************
* 
* @file      SoundData.h
* @brief     �T�E���h�f�[�^
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      �T�E���h��񋓂���
* @attention 
* 
******************************************************************************/
#pragma once
enum eBGM {
	BGM_GAME = 0,	// �Q�[��BGM
	BGM_TITLE,		// �^�C�g��BGM

	MAX_BGM
};

enum eSE {
	SE_SHOT = 0,	// �e���ˉ�
	SE_EXPLOSION,	// ������
	SE_DEFEND,		// �Փˉ�
	SE_GAMEOVER,	// �Q�[���I�[�o�[BGM

	MAX_SE
};

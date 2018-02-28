#pragma once
#include <AntTweakBar.h>


class Game;


class FurDemoUI
{
public:
	FurDemoUI(Game& _game);
	FurDemoUI() = default;

private:
	Game* m_game {nullptr};

	TwBar* m_fur_bar{ nullptr };//memory managed by AntTweak
	TwBar* m_light_bar{ nullptr };
	TwBar* m_model_bar{ nullptr };
	TwBar* m_texture_bar{ nullptr };
	TwBar* m_help_bar{ nullptr };

	TwType m_tw_model_type;
	TwType m_tw_texture_type;
	TwType m_tw_fur_alpha_texture_type;
	TwType m_tw_fur_mask_texture_type;

	void CreateDebugUI();
	void CreateFurBar();
	void CreateFurBarElements() const;
	void CreateLightBar();
	void CreateLightBarElements() const;
	void CreateModelBar();
	void CreateModelBarElements();
	void CreateHelpBar();
	void CreateTextureBar();
	void CreateTextureBarElements();
	void CreateTextureSetElement();
	void CreateTextureAlphasElement();
	void CreateTextureMasksElement();
};

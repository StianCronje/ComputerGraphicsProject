#ifndef _MENUSTATE_H_
#define _MENUSTATE_H_

#include "GameFont.h."

//This displays the menu so the player
// can chose to play a new game, continue to existing game,
//see the high score or exit the game.

class CMenuState : // public ____________ --> fix
{
public:
	~CMenuState();

	void OnKeyDown(WPARAM wKey);
	void Draw();
	void EnterState();

private:
	// The player went up or down in 
	// the menu
	void SelectionUp();
	void SelectionDown();
	// The player validated the current selection
	void SelectionChosen();

	CGameFont* m_pFont;
	// Index of the current selected menu item
	int m_iCurrentSelection;

	// A pointer to the current active game (if any).
	CPlayState* m_pCurrentGame;

	// The background and title images
	TImagePtr m_pBackgroundImg;
	TImagePtr m_pTitleImg;

	// The images of the menu items (normal and
	// selected).
	TImagePtr m_pItemBckgndNormal;
	TImagePtr m_pItemBckgndSelected;

	// The text controls of the different entries.
	CTextControl* m_pNewGameText;
	CTextControl* m_pResumeGameText;
	CTextControl* m_pScoresText;
	CTextControl* m_pExitText;
};

#endif  // _MENUSTATE_H_

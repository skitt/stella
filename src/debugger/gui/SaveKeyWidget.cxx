//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2017 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

#include "SaveKey.hxx"
#include "MT24LC256.hxx"
#include "SaveKeyWidget.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
SaveKeyWidget::SaveKeyWidget(GuiObject* boss, const GUI::Font& font,
                             int x, int y, Controller& controller)
  : FlashWidget(boss, font, x, y, controller)
{
  init(boss, font, x, y);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SaveKeyWidget::eraseCurrent()
{
  SaveKey& skey = static_cast<SaveKey&>(myController);

  skey.myEEPROM->eraseCurrent();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool SaveKeyWidget::isPageUsed(uInt32 page)
{
  SaveKey& skey = static_cast<SaveKey&>(myController);

  return skey.myEEPROM->isPageUsed(page);
}

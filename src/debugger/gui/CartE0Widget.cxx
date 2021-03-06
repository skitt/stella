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
// Copyright (c) 1995-2020 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

#include "CartE0.hxx"
#include "PopUpWidget.hxx"
#include "CartE0Widget.hxx"

static constexpr std::array<const char*, 8> seg0 = {
  "0 ($FFE0)", "1 ($FFE1)", "2 ($FFE2)", "3 ($FFE3)",
  "4 ($FFE4)", "5 ($FFE5)", "6 ($FFE6)", "7 ($FFE7)"
};
static constexpr std::array<const char*, 8> seg1 = {
  "0 ($FFE8)", "1 ($FFE9)", "2 ($FFEA)", "3 ($FFEB)",
  "4 ($FFEC)", "5 ($FFED)", "6 ($FFEE)", "7 ($FFEF)"
};
static constexpr std::array<const char*, 8> seg2 = {
  "0 ($FFF0)", "1 ($FFF1)", "2 ($FFF2)", "3 ($FFF3)",
  "4 ($FFF4)", "5 ($FFF5)", "6 ($FFF6)", "7 ($FFF7)"
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CartridgeE0Widget::CartridgeE0Widget(
      GuiObject* boss, const GUI::Font& lfont, const GUI::Font& nfont,
      int x, int y, int w, int h, CartridgeE0& cart)
  : CartDebugWidget(boss, lfont, nfont, x, y, w, h),
    myCart(cart)
{
  uInt32 size = 8 * 1024;

  string info =
    "E0 cartridge, eight 1K slices\n"
    "Segment 0 accessible @ $F000 - $F3FF\n"
    "  Hotspots $FE0 to $FE7\n"
    "Segment 1 accessible @ $F400 - $F7FF\n"
    "  Hotspots $FE8 to $FEF\n"
    "Segment 2 accessible @ $F800 - $FBFF\n"
    "  Hotspots $FF0 to $FF7\n"
    "Segment 3 accessible @ $FC00 - $FFFF\n"
    "  Always points to last 1K of ROM\n"
    "Startup slices = 4 / 5 / 6 or undetermined\n";

#if 0
  // Eventually, we should query this from the debugger/disassembler
  uInt16 start = (cart.myImage[size-3] << 8) | cart.myImage[size-4];
  start -= start % 0x1000;
  info << "Bank RORG" << " = $" << HEX4 << start << "\n";
#endif
  int xpos = 2,
      ypos = addBaseInformation(size, "Parker Brothers", info) + myLineHeight;

  VariantList items0, items1, items2;
  for(int i = 0; i < 8; ++i)
  {
    VarList::push_back(items0, seg0[i]);
    VarList::push_back(items1, seg1[i]);
    VarList::push_back(items2, seg2[i]);
  }

  const int lwidth = _font.getStringWidth("Set slice for segment X ");
  mySlice0 =
    new PopUpWidget(boss, _font, xpos, ypos-2, _font.getStringWidth("7 ($FFF7)"),
                    myLineHeight, items0, "Set slice for segment 0 ",
                    lwidth, kSlice0Changed);
  mySlice0->setTarget(this);
  addFocusWidget(mySlice0);
  ypos += mySlice0->getHeight() + 4;

  mySlice1 =
    new PopUpWidget(boss, _font, xpos, ypos-2, _font.getStringWidth("7 ($FFF7)"),
                    myLineHeight, items1, "Set slice for segment 1 ",
                    lwidth, kSlice1Changed);
  mySlice1->setTarget(this);
  addFocusWidget(mySlice1);
  ypos += mySlice1->getHeight() + 4;

  mySlice2 =
    new PopUpWidget(boss, _font, xpos, ypos-2, _font.getStringWidth("7 ($FFF7)"),
                    myLineHeight, items2, "Set slice for segment 2 ",
                    lwidth, kSlice2Changed);
  mySlice2->setTarget(this);
  addFocusWidget(mySlice2);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeE0Widget::loadConfig()
{
  mySlice0->setSelectedIndex(myCart.myCurrentSlice[0]);
  mySlice1->setSelectedIndex(myCart.myCurrentSlice[1]);
  mySlice2->setSelectedIndex(myCart.myCurrentSlice[2]);

  CartDebugWidget::loadConfig();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeE0Widget::handleCommand(CommandSender* sender,
                                      int cmd, int data, int id)
{
  myCart.unlockBank();

  switch(cmd)
  {
    case kSlice0Changed:
      myCart.segmentZero(mySlice0->getSelected());
      break;
    case kSlice1Changed:
      myCart.segmentOne(mySlice1->getSelected());
      break;
    case kSlice2Changed:
      myCart.segmentTwo(mySlice2->getSelected());
      break;
    default:
      break;
  }

  myCart.lockBank();
  invalidate();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
string CartridgeE0Widget::bankState()
{
  ostringstream& buf = buffer();

  buf << "Slices: " << std::dec
      << seg0[myCart.myCurrentSlice[0]] << " / "
      << seg1[myCart.myCurrentSlice[1]] << " / "
      << seg2[myCart.myCurrentSlice[2]];

  return buf.str();
}

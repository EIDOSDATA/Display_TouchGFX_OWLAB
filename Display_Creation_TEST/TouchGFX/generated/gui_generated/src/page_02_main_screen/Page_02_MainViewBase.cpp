/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/page_02_main_screen/Page_02_MainViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

Page_02_MainViewBase::Page_02_MainViewBase() :
    buttonCallback(this, &Page_02_MainViewBase::buttonCallbackHandler)
{
    __background.setPosition(0, 0, 1024, 600);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    box1.setPosition(0, 0, 1024, 600);
    box1.setColor(touchgfx::Color::getColorFromRGB(61, 81, 235));
    add(box1);

    PREV_BTN_MAIN.setXY(49, 500);
    PREV_BTN_MAIN.setBitmaps(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_BUTTON_REGULAR_HEIGHT_50_MEDIUM_ROUNDED_NORMAL_ID), touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_BUTTON_REGULAR_HEIGHT_50_MEDIUM_ROUNDED_PRESSED_ID));
    PREV_BTN_MAIN.setLabelText(touchgfx::TypedText(T___SINGLEUSE_UENW));
    PREV_BTN_MAIN.setLabelColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    PREV_BTN_MAIN.setLabelColorPressed(touchgfx::Color::getColorFromRGB(255, 255, 255));
    PREV_BTN_MAIN.setAction(buttonCallback);
    add(PREV_BTN_MAIN);
}

Page_02_MainViewBase::~Page_02_MainViewBase()
{

}

void Page_02_MainViewBase::setupScreen()
{

}

void Page_02_MainViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &PREV_BTN_MAIN)
    {
        //PREV_BTN_Interaction
        //When PREV_BTN_MAIN clicked change screen to Page_01_Loading
        //Go to Page_01_Loading with screen transition towards West
        application().gotoPage_01_LoadingScreenSlideTransitionWest();
    }
}

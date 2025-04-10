/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef PAGE_02_MAINVIEWBASE_HPP
#define PAGE_02_MAINVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/page_02_main_screen/Page_02_MainPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>

class Page_02_MainViewBase : public touchgfx::View<Page_02_MainPresenter>
{
public:
    Page_02_MainViewBase();
    virtual ~Page_02_MainViewBase();
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::Box box1;
    touchgfx::ButtonWithLabel PREV_BTN_MAIN;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<Page_02_MainViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

};

#endif // PAGE_02_MAINVIEWBASE_HPP

#ifndef PAGE_02_MAINVIEW_HPP
#define PAGE_02_MAINVIEW_HPP

#include <gui_generated/page_02_main_screen/Page_02_MainViewBase.hpp>
#include <gui/page_02_main_screen/Page_02_MainPresenter.hpp>

class Page_02_MainView : public Page_02_MainViewBase
{
public:
    Page_02_MainView();
    virtual ~Page_02_MainView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // PAGE_02_MAINVIEW_HPP

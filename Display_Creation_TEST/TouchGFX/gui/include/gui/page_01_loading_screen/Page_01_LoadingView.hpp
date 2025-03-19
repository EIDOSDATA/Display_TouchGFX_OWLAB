#ifndef PAGE_01_LOADINGVIEW_HPP
#define PAGE_01_LOADINGVIEW_HPP

#include <gui_generated/page_01_loading_screen/Page_01_LoadingViewBase.hpp>
#include <gui/page_01_loading_screen/Page_01_LoadingPresenter.hpp>

class Page_01_LoadingView : public Page_01_LoadingViewBase
{
public:
    Page_01_LoadingView();
    virtual ~Page_01_LoadingView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // PAGE_01_LOADINGVIEW_HPP

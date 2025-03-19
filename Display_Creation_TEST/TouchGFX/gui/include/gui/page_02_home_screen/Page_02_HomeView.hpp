#ifndef PAGE_02_HOMEVIEW_HPP
#define PAGE_02_HOMEVIEW_HPP

#include <gui_generated/page_02_home_screen/Page_02_HomeViewBase.hpp>
#include <gui/page_02_home_screen/Page_02_HomePresenter.hpp>

class Page_02_HomeView : public Page_02_HomeViewBase
{
public:
    Page_02_HomeView();
    virtual ~Page_02_HomeView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // PAGE_02_HOMEVIEW_HPP

#ifndef PAGE_00_LOGOVIEW_HPP
#define PAGE_00_LOGOVIEW_HPP

#include <gui_generated/page_00_logo_screen/Page_00_LogoViewBase.hpp>
#include <gui/page_00_logo_screen/Page_00_LogoPresenter.hpp>

class Page_00_LogoView : public Page_00_LogoViewBase
{
public:
    Page_00_LogoView();
    virtual ~Page_00_LogoView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // PAGE_00_LOGOVIEW_HPP

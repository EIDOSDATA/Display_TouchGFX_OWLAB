#ifndef PAGE_00_LOGOPRESENTER_HPP
#define PAGE_00_LOGOPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Page_00_LogoView;

class Page_00_LogoPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Page_00_LogoPresenter(Page_00_LogoView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~Page_00_LogoPresenter() {}

private:
    Page_00_LogoPresenter();

    Page_00_LogoView& view;
};

#endif // PAGE_00_LOGOPRESENTER_HPP

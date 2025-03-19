#ifndef PAGE_01_LOADINGPRESENTER_HPP
#define PAGE_01_LOADINGPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Page_01_LoadingView;

class Page_01_LoadingPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Page_01_LoadingPresenter(Page_01_LoadingView& v);

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

    virtual ~Page_01_LoadingPresenter() {}

private:
    Page_01_LoadingPresenter();

    Page_01_LoadingView& view;
};

#endif // PAGE_01_LOADINGPRESENTER_HPP

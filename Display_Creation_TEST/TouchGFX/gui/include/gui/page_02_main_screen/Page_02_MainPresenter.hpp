#ifndef PAGE_02_MAINPRESENTER_HPP
#define PAGE_02_MAINPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Page_02_MainView;

class Page_02_MainPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Page_02_MainPresenter(Page_02_MainView& v);

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

    virtual ~Page_02_MainPresenter() {}

private:
    Page_02_MainPresenter();

    Page_02_MainView& view;
};

#endif // PAGE_02_MAINPRESENTER_HPP

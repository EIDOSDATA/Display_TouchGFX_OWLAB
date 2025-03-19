#ifndef PAGE_02_HOMEPRESENTER_HPP
#define PAGE_02_HOMEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Page_02_HomeView;

class Page_02_HomePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Page_02_HomePresenter(Page_02_HomeView& v);

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

    virtual ~Page_02_HomePresenter() {}

private:
    Page_02_HomePresenter();

    Page_02_HomeView& view;
};

#endif // PAGE_02_HOMEPRESENTER_HPP

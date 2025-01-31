#pragma once
#define DISABLE_ALL_LIBRARY_WARNINGS
#include <TFT_eSPI.h>
#include "events/event.h"
#include "events/event_manager.h"
#include "ui/page.h"

extern TFT_eSPI tft;

enum PageType {
    MAIN_MENU,
    SINGLE_EFFECT,
    ORDERED_EFFECTS,
    SETTINGS
};

class UIManager {
private:
    static PageType currentPage;
public:
    static Page* pages[];

    static void init();
    static void handleEvent(const Event& event);
    static void changePage(PageType page);
};
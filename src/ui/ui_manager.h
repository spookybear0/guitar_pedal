#pragma once
#define DISABLE_ALL_LIBRARY_WARNINGS
#include <TFT_eSPI.h>
#include "events/event.h"
#include "events/event_manager.h"
#include "ui/page.h"

extern TFT_eSPI tft;

class UIManager {
public:
    static Page* pages[];
    static int currentPage;

    static void init();
    static void handleEvent(const Event& event);
};
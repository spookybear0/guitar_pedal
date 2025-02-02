#include <ui/pages/ordered_effects.h>
#include "ui/page.h"
#define DISABLE_ALL_LIBRARY_WARNINGS
#include "TFT_eSPI.h"
#include "ui/ui_manager.h"
#include "events/event_manager.h"
#include "input.h"

void OrderedEffectsPage::draw() {
    // not implemented yet
}

void OrderedEffectsPage::handleEvent(const Event& event) {
    // not implemented yet
}

void OrderedEffectsPage::enter() {
    // go back to main menu since not implemented yet
    UIManager::changePage(PageType::MAIN_MENU);
}
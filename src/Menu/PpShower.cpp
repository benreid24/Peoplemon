#include "Menu/PpShower.hpp"
#include "Game/Game.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

PpShower::PpShower(Game* g, MoveRef* mvs) : game(g), moves(mvs) {
    bgnd.setImage("ppshowerbgnd.png");
    name.setProps(Color::Black, 32);
    type.setProps(Color::Blue, 24);

    bgnd.setPosition(Vector2f(505,475));
    name.setPosition(Vector2f(510,480));
    type.setPosition(Vector2f(520,510));
    pp.setPosition(Vector2f(512,533));

    menu.add(&bgnd);
    menu.add(&name);
    menu.add(&type);
    menu.add(&pp);

    for (int i = 0; i<4; ++i) {
        if (moves[i].id != 0)
            updateText(moves[i]);
    }
}

void PpShower::updateText(const MoveRef& rf) {
    const Move& mv = game->moveList[rf.id];

    name.setText(mv.name);
    type.setText(moveTypeToString(mv.type));
    pp.setText(intToString(rf.curPp)+"/"+intToString(mv.pp));

    double prop = double(rf.curPp) / double(mv.pp);
    if (prop >= 0.6)
        pp.setProps(Color(50,255,50), 20);
    else if (prop >= 0.25)
        pp.setProps(Color(181,181,0), 20);
    else
        pp.setProps(Color::Red, 20);
}

void PpShower::updateText(const string& nm) {
    for (int i = 0; i<4; ++i) {
        if (game->moveList[moves[i].id].name == nm) {
            updateText(moves[i]);
            return;
        }
    }
    name.setText("");
    type.setText("");
    pp.setText("");
}

void PpShower::draw(RenderWindow* window) {
    menu.draw(window);
}

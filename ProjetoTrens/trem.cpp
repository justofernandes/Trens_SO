// trem.cpp
#include "trem.h"
#include <algorithm>
#include <vector>
#include <QThread>

// Definições estáticas
QMutex Trem::arbiter;
QMutex* Trem::regiao1 = nullptr;
QMutex* Trem::regiao2 = nullptr;
QMutex* Trem::regiao3 = nullptr;
QMutex* Trem::regiao4 = nullptr;
QMutex* Trem::regiao5 = nullptr;
QMutex* Trem::regiao6 = nullptr;
QMutex* Trem::regiao7 = nullptr;

// Mapear id -> ponteiro do mutex
static inline QMutex* regionPtrById(int id) {
    switch (id) {
    case 1: return Trem::regiao1;
    case 2: return Trem::regiao2;
    case 3: return Trem::regiao3;
    case 4: return Trem::regiao4;
    case 5: return Trem::regiao5;
    case 6: return Trem::regiao6;
    case 7: return Trem::regiao7;
    default: return nullptr;
    }
}

void Trem::lockRegions(const std::vector<int>& regs) {
    if (regs.empty()) {
        return;
    }

    // Se for apenas uma região, travar direto (melhora concorrência)
    if (regs.size() == 1) {
        QMutex* m = regionPtrById(regs[0]);
        if (m) m->lock();
        return;
    }

    // Caso múltiplas: ordenar e usar arbiter para evitar deadlock circular
    std::vector<int> ordered = regs;
    std::sort(ordered.begin(), ordered.end());

    Trem::arbiter.lock();
    for (int id : ordered) {
        QMutex* m = regionPtrById(id);
        if (m) m->lock();
    }
    Trem::arbiter.unlock();
}

void Trem::unlockRegions(const std::vector<int>& regs) {
    if (regs.empty()){
        return;
    }

    std::vector<int> ordered = regs;
    std::sort(ordered.begin(), ordered.end(), std::greater<int>()); // libera em ordem reversa
    for (int id : ordered) {
        QMutex* m = regionPtrById(id);
        if (m) m->unlock();
    }
    QThread::msleep(1);
}

// Construtor e setters simples
Trem::Trem(int id, int startX, int startY) {
    ID = id;
    x = startX;
    y = startY;
    velocidade = 50;
}

void Trem::setVelocidade(int velocidade) {
    if(velocidade == 0){
        this->velocidade = 1000000;
    } else {
        this->velocidade = 200 - velocidade;
    }
}

void Trem::right() {
    x += 10;
}
void Trem::left() {
    x -= 10;
}
void Trem::up() {
    y -= 10;
}
void Trem::down() {
    y += 10;
}

void Trem::run() {
    bool inReg1 = false, inReg2 = false, inReg3 = false, inReg4 = false;
    bool inReg5 = false, inReg6 = false, inReg7 = false;

    while (true) {
        switch (ID) {
        case 1:
            if (y == 30 && x < 200) {
                if (x == 180 && !inReg1) {
                    lockRegions({1});
                    inReg1 = true;
                }
                right();
            } else if (x == 200 && y < 150) {
                if (y == 130 && !inReg3) {
                    lockRegions({3});
                    inReg3 = true;
                }
                down();
            } else if (x > 60 && y == 150) {
                if (x == 180 && inReg1) {
                    unlockRegions({1});
                    inReg1 = false;
                }
                if (x == 110 && inReg3) {
                    unlockRegions({3});
                    inReg3 = false;
                }
                left();
            } else {
                up();
            }
            break;

        case 2:
            if (y == 30 && x < 340) {
                if (!inReg1 && !inReg2 && x >= 200 && x <= 320) {
                    lockRegions({1,2});
                    inReg1 = inReg2 = true;
                }
                else if (x == 220 && inReg1) {
                    unlockRegions({1});
                    inReg1 = false;
                }
                right();
            } else if (x == 340 && y < 150) {
                if (!inReg2 && !inReg5) {
                    lockRegions({2,5});
                    inReg2 = inReg5 = true;
                }
                else if (!inReg5 && inReg2) {
                    lockRegions({5});
                    inReg5 = true;
                }
                down();
            } else if (x > 200 && y == 150) {
                if (!inReg4 && x == 290) {
                    lockRegions({4});
                    inReg4 = true;
                }
                if (x == 320 && inReg2) {
                    unlockRegions({2});
                    inReg2 = false;
                }
                if (x == 250 && inReg5) {
                    unlockRegions({5});
                    inReg5 = false;
                }
                if (x == 220 && !inReg1) {
                    lockRegions({1});
                    inReg1 = true;
                }
                left();
            } else {
                if (x == 200 && y == 130 && inReg4) {
                    unlockRegions({4});
                    inReg4 = false;
                }
                up();
            }

            if (!inReg1 && !inReg2 && !inReg4 && !inReg5 && x >= 200 && x <= 320 && y >= 30 && y <= 150) {
                lockRegions({1,2,4,5});
                inReg1 = inReg2 = inReg4 = inReg5 = true;
            }
            break;

        case 3:
            if (y == 30 && x < 480) {
                if (x == 360 && inReg2) {
                    unlockRegions({2});
                    inReg2 = false;
                }
                right();
            } else if (x == 480 && y < 150) {
                down();
            } else if (x > 340 && y == 150) {
                if (!inReg2 && !inReg6) {
                    lockRegions({2,6});
                    inReg2 = inReg6 = true;
                }
                else {
                    if (x == 430 && !inReg6) {
                        lockRegions({6});
                        inReg6 = true;
                    }
                    if (x == 360 && !inReg2) {
                        lockRegions({2});
                        inReg2 = true;
                    }
                }
                left();
            } else {
                if (x == 340 && y == 130 && inReg6) {
                    unlockRegions({6});
                    inReg6 = false;
                }
                up();
            }
            break;

        case 4:
            if (y == 150 && x < 270) {
                if (!inReg4 && x == 180) {
                    lockRegions({4});
                    inReg4 = true;
                }
                if (!inReg7 && x == 250) {
                    lockRegions({7});
                    inReg7 = true;
                }
                if (x == 220 && inReg3) {
                    unlockRegions({3});
                    inReg3 = false;
                }
                right();
            } else if (x == 270 && y < 280) {
                if (y == 170 && inReg4) {
                    unlockRegions({4});
                    inReg4 = false;
                }
                down();
            } else if (x > 130 && y == 280) {
                if (x == 250 && inReg7) {
                    unlockRegions({7});
                    inReg7 = false;
                }
                left();
            } else {
                if (x == 130 && y == 170 && !inReg3) {
                    lockRegions({3});
                    inReg3 = true;
                }
                up();
            }

            if (!inReg3 && !inReg4 && !inReg7 && x >= 130 && x <= 270 && y >= 150 && y <= 280) {
                lockRegions({3,4,7});
                inReg3 = inReg4 = inReg7 = true;
            }
            break;

        case 5:
            if (y == 150 && x < 410) {
                if (x == 290 && inReg7) {
                    unlockRegions({7});
                    inReg7 = false;
                }
                if (x == 320 && !inReg6) {
                    lockRegions({6});
                    inReg6 = true;
                }
                if (x == 360 && inReg5) {
                    unlockRegions({5});
                    inReg5 = false;
                }
                right();
            } else if (x == 410 && y < 280) {
                if (y == 170 && inReg6) {
                    unlockRegions({6});
                    inReg6 = false;
                }
                down();
            } else if (x > 270 && y == 280) {
                if (x == 290 && !inReg7) {
                    lockRegions({7});
                    inReg7 = true;
                }
                left();
            } else {
                if (x == 270 && y == 170 && !inReg5) {
                    lockRegions({5});
                    inReg5 = true;
                }
                up();
            }

            if (!inReg5 && !inReg6 && !inReg7 && x >= 270 && x <= 410 && y >= 150 && y <= 280) {
                lockRegions({5,6,7});
                inReg5 = inReg6 = inReg7 = true;
            }
            break;

        case 6:
            if (y == 30 && x > 60) {
                if (x == 360 && !inReg2) {
                    lockRegions({2});
                    inReg2 = true;
                }
                if (x == 340 && inReg2) {
                    unlockRegions({2});
                    inReg2 = false;
                }
                if (x == 220 && !inReg1) {
                    lockRegions({1});
                    inReg1 = true;
                }
                if (x == 200 && inReg1) {
                    unlockRegions({1});
                    inReg1 = false;
                }
                left();
            } else if (x == 60 && y < 280) {
                down();
            } else if (x < 480 && y == 280) {
                if (x == 250 && !inReg7) {
                    lockRegions({7});
                    inReg7 = true;
                }
                if (x == 270 && inReg7) {
                    unlockRegions({7});
                    inReg7 = false;
                }
                right();
            } else {
                up();
            }

            if (!inReg1 && !inReg2 && x >= 200 && x <= 360 && y <= 30) {
                lockRegions({1,2});
                inReg1 = inReg2 = true;
            }
            break;

        default:
            break;
        }

        emit updateGUI(ID, x, y);
        QThread::msleep(velocidade);
    }
}


#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <QApplication>
#include <QWidget>
#include <vector>
#include <random>
#include <cmath>
#include <cstdlib>
#include <iostream>

struct Vec2 {
    double x, y;

    Vec2 operator+(const Vec2& b) const {
        return {x + b.x, y + b.y};
    }

    Vec2 operator-(const Vec2& b) const {
        return {x - b.x, y - b.y};
    }

    Vec2 operator*(double a) const {
        return {x * a, y * a};
    }

    Vec2& operator+=(const Vec2& b) {
        x += b.x;
        y += b.y;
        return *this;
    }
};

double norm(const Vec2& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

// define simulation parameters
const int N = 1000;
const double WORLD_SIZE = 100.0;

const double VISUAL_RANGE = 40.0;
const double PROTECTED_RANGE = 8.0;

const double CENTERING_FACTOR = 0.0005;
const double MATCHING_FACTOR = 0.05;
const double AVOID_FACTOR = 0.05;
const double TURN_FACTOR = 0.2;

const double MIN_SPEED = 3.0;
const double MAX_SPEED = 6.0;

const double LEFT_MARGIN = 200.0;
const double RIGHT_MARGIN = WORLD_SIZE - 200.0;
const double TOP_MARGIN = 200.0;
const double BOTTOM_MARGIN = WORLD_SIZE - 200.0;

// global rendering state
std::vector<Vec2> pos(N);
std::vector<Vec2> vel(N);

std::vector<SoTranslation*> boidTranslations(N, nullptr);

SoCoordinate3* coordinates = nullptr;
SoPointSet* points = nullptr;

// initialize the flock
void initializeFlock() {
    std::mt19937 rng(std::random_device{}());

    std::normal_distribution<double> positionDist(0.0, 100.0);
    std::normal_distribution<double> angleDist(0.0, 0.3);

    for (int i = 0; i < N; ++i) {
        pos[i] = {
            WORLD_SIZE / 2.0 + positionDist(rng),
            WORLD_SIZE / 2.0 + positionDist(rng)
        };

        double angle = angleDist(rng);

        vel[i] = {
            0.03 * std::cos(angle),
            0.03 * std::sin(angle)
        };
    }
}

// advance one simulation step
void updateBoids() {
    const double visual2 = VISUAL_RANGE * VISUAL_RANGE;
    const double protected2 = PROTECTED_RANGE * PROTECTED_RANGE;

    // apply separation to every bird.
    for (int i = 0; i < N; ++i) {
        Vec2 close{0.0, 0.0};

        for (int j = 0; j < N; ++j) {
            if (i == j) continue;

            Vec2 diff = pos[i] - pos[j];
            double d2 = diff.x * diff.x + diff.y * diff.y;

            if (d2 < protected2) {
                close += diff;
            }
        }

        vel[i] += close * AVOID_FACTOR;
    }

    // cohesion and alignment
    for (int i = 0; i < N; ++i) {
        Vec2 averagePos{0.0, 0.0};
        Vec2 averageVel{0.0, 0.0};
        int count = 0;

        for (int j = 0; j < N; ++j) {
            if (i == j) continue;

            Vec2 diff = pos[i] - pos[j];
            double d2 = diff.x * diff.x + diff.y * diff.y;

            if (d2 < visual2 && d2 >= protected2) {
                averagePos += pos[j];
                averageVel += vel[j];
                ++count;
            }
        }

        if (count > 0) {
            double invCount = 1.0 / count;

            averagePos = averagePos * invCount;
            averageVel = averageVel * invCount;

            vel[i] += (averagePos - pos[i]) * CENTERING_FACTOR;
            vel[i] += (averageVel - vel[i]) * MATCHING_FACTOR;
        }
    }

    // boundary conditions
    for (int i = 0; i < N; ++i) {
        if (pos[i].y < TOP_MARGIN)
            vel[i].y += TURN_FACTOR;

        if (pos[i].y > BOTTOM_MARGIN)
            vel[i].y -= TURN_FACTOR;

        if (pos[i].x < LEFT_MARGIN)
            vel[i].x += TURN_FACTOR;

        if (pos[i].x > RIGHT_MARGIN)
            vel[i].x -= TURN_FACTOR;
    }

    // speed limits and update positions
    for (int i = 0; i < N; ++i) {
        double speed = norm(vel[i]);

        if (speed > 0.0 && speed < MIN_SPEED) {
            vel[i] = vel[i] * (MIN_SPEED / speed);
        } else if (speed > MAX_SPEED) {
            vel[i] = vel[i] * (MAX_SPEED / speed);
        }

        pos[i] += vel[i];
    }
}


void updateDisplay() {
    for (int i = 0; i < N; ++i) {
        boidTranslations[i]->translation.setValue(
            static_cast<float>(pos[i].x),
            static_cast<float>(pos[i].y),
            0.0f
        );
    }
}

// snimation callback
void callback(void*, SoSensor*) {
    updateBoids();
    updateDisplay();
}

// MAIN
int main(int argc, char** argv) {
#ifndef __APPLE__
    setenv("QT_QPA_PLATFORM", "xcb", 0);
#endif

    QApplication app(argc, argv);
    QWidget mainwin;

    SoQt::init(&mainwin);

    initializeFlock();

    // sceme
    SoSeparator* root = new SoSeparator;
    root->ref();

    // plot
    SoLightModel* lightModel = new SoLightModel;
    lightModel->model = SoLightModel::BASE_COLOR;
    root->addChild(lightModel);


    // sphere appearance
    SoMaterial* material = new SoMaterial;
    material->diffuseColor.setValue(1.0f, 1.0f, 1.0f);
    material->specularColor.setValue(1.0f, 1.0f, 1.0f);
    material->shininess = 0.6f;
    root->addChild(material);

    // create one sphere per boid
    for (int i = 0; i < N; ++i) {
        SoSeparator* boidSep = new SoSeparator;

        // position of this boid
        SoTranslation* translation = new SoTranslation;
        translation->translation.setValue(
            static_cast<float>(pos[i].x),
            static_cast<float>(pos[i].y),
            0.0f
        );

        boidTranslations[i] = translation;

        // Sphere geometry
        SoSphere* sphere = new SoSphere;
        sphere->radius = 3.0f;

        boidSep->addChild(translation);
        boidSep->addChild(sphere);

        root->addChild(boidSep);
    }


    // viewer
    SoQtExaminerViewer* viewer = new SoQtExaminerViewer(&mainwin);

    viewer->setSceneGraph(root);
    viewer->setBackgroundColor(SbColor(0.016f, 0.071f, 0.212f));
    viewer->show();
    viewer->viewAll();

    // use a timer to advance the animation.
    SoTimerSensor* timer = new SoTimerSensor;
    timer->setFunction(callback);
    timer->setInterval(SbTime(1.0 / 60.0));
    timer->schedule();

    SoQt::show(&mainwin);
    SoQt::mainLoop();

    delete timer;
    delete viewer;
    root->unref();

    return 0;
}
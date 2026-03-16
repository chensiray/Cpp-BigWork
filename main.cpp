#include "Button.h"
#include "Player.h"
#include "Bullet.h"
#include "BulletBox.h"
#include "rescource.h"
int main() {
    //窗口初始化
    sf::RenderWindow window(sf::VideoMode({ 1920,1080 }), "My Windows", sf::Style::Default);
    window.setFramerateLimit(60);
    sf::View gameview(sf::FloatRect({ 0,0 }, { 1920,1080 }));
    updateViewport(window, gameview);
    window.setVerticalSyncEnabled(true);
    sf::Clock gameClock;
    sf::Time levelTime;
    sf::Time lastTime, currentTime;
    bool isPaused = false;
    //关卡阶段
    int level = -1;
    int stage = 0;
    //按钮
    sf::String start = U"开始游戏";
    sf::String pause = U"继续游戏";
    Button startButton(start, {300.f,120.f}, {600.f,600.f});
    Button pauseButton(pause, { 320.f,100.f }, { 960.f,540.f });
    //玩家
    Player player(0, 100.f, 100.f);
    //子弹
    std::vector<std::unique_ptr<Bullet>> bullets;
    BulletBox box1 = BulletBox({ 480.f,360.f }, bullets);//左上子弹源
    BulletBox box2 = BulletBox({ 1440.f,360.f }, bullets);//右上子弹源
    BulletBox boxleft = BulletBox({ 20.f,540.f }, bullets);//左侧子弹源
    BulletBox boxright = BulletBox({ 1900.f,540.f }, bullets);//右侧子弹源
    BulletBox boxup = BulletBox({ 960.f,20.f }, bullets);
    BulletBox boxdown = BulletBox({ 960.f,1060.f }, bullets);
    //主循环
    while (window.isOpen()) {
        //事件循环
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())//关闭窗口
            {
                std::cout << "游戏时间:" << gameTime / sf::seconds(1.f) << "秒" << std::endl;
                window.close();
            }
            if (event->is<sf::Event::Resized>())//调整窗口大小
            {
                updateViewport(window, gameview);
                sf::Vector2f viewsize = gameview.getSize();
            }
            if (isPaused)
            {
                if (pauseButton.headleEvent(*event, window))
                {
                    isPaused = false;
                }
            }
            if (level == -1)
            {
                if (startButton.headleEvent(*event, window))//开始游戏
                {
                    std::cout << "开始游戏！" << std::endl;
                    player.addlife(3);
                    level = 0;
                    levelTime = sf::seconds(0.f);
                }
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::P)
                {
                    isPaused = !isPaused;
                }
            }
        }
        //更新循环
        window.setView(gameview);
        window.clear(sf::Color::Black);
        currentTime = gameClock.getElapsedTime();
        if (!isPaused)
        {
            gameTime += currentTime - lastTime;
            levelTime += currentTime - lastTime;
        }
        lastTime = currentTime;
        //主菜单
        if (level == -1 || level == 0 )
        {
            sf::Vector2u windowSize = window.getSize();
            if (windowSize.x > 0 && windowSize.y > 0)
            {
                sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
                sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);
                startButton.update(mouseWorld);
            }
            startButton.draw(window, sf::RenderStates::Default);
            if (level == 0 && levelTime / sf::seconds(1.f) > 0.5f)
            {
                level = 1;
            }
        }
        //暂停
        if (isPaused)
        {
            sf::Vector2u windowSize = window.getSize();
            if (windowSize.x > 0 && windowSize.y > 0)
            {
                sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
                sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);
                pauseButton.update(mouseWorld);
            }
            pauseButton.draw(window, sf::RenderStates::Default);
        }
        //子弹和玩家更新
        if (!isPaused) player.update();
        for (size_t i = 0; i < bullets.size(); i++)
        {
            auto& bullet = bullets[i];
            if (!isPaused)
            {
                bullet->update();
            }
        }
        if (!isPaused)
        {
            for (auto& bullet : bullets)
            {
                if (bullet->checkCollision(player.getBounds()))
                {
                    bullet->isactive = false;
                    player.hurt();
                }
            }
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const std::unique_ptr<Bullet>& bullet) {return !bullet->isactive; }), bullets.end()); 
        for (auto& bullet : bullets)
        {
            bullet->draw(window, sf::RenderStates::Default);
        }
        player.draw(window, sf::RenderStates::Default);
        window.display();
        //第一关
        if (level == 1)
        {
            if (stage == 0 && levelTime / sf::seconds(1.f) > 3.f)//阶段0
            {
                box1.round(20.f, 2, 200.f);
                box2.round(20.f, 2, 200.f);
                box1.round(20.f, 2, 0);
                box2.round(20.f, 2, 0);
                box2.follow(40.f, 1, &player);
                box1.follow(40.f, 1, &player);
                stage = 1;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 1 && levelTime / sf::seconds(1.f) > 2.f)//阶段1
            {
                box1.round(20.f, 2, 200.f);
                box2.round(20.f, 2, 200.f);
                box1.round(20.f, 2, 0);
                box2.round(20.f, 2, 0);
                box2.follow(40.f, 2, &player);
                box1.follow(40.f, 2, &player);
                stage = 2;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 2 && levelTime / sf::seconds(1.f) > 2.f)//阶段2
            {
                boxleft.rightround(20.f, 2);
                box2.follow(40.f, 1,&player);
                stage = 3;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 3 && levelTime / sf::seconds(1.f) > 2.f)//阶段3
            {
                boxright.leftround(20.f, 2);
                box1.follow(40.f, 1, &player);
                stage = 4;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 4 && levelTime / sf::seconds(1.f) > 4.f)
            {
                boxleft.randomfall({ 200.f,140.f }, 2, 0);
                box2.follow(40.f, 2, &player);
                stage = 5;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 5 && levelTime / sf::seconds(1.f) > 8.f)
            {
                boxright.randomfall({ 200.f,140.f }, 2, pi);
                box1.follow(40.f, 2, &player);
                stage = 6;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 6 && levelTime / sf::seconds(1.f) > 8.f)
            {
                boxleft.randomfall({ 200.f,140.f }, 2, 0);
                boxright.randomfall({ 200.f,140.f }, 2, pi);
                boxup.round(20.f, 3, 100.f);
                boxdown.round(20.f, 3, 100.f);
                stage = 7;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 7 && levelTime / sf::seconds(1.f) > 8.f)
            {
                boxleft.randomfall({ 200.f,140.f }, 2, 0);
                boxright.randomfall({ 200.f,140.f }, 2, pi);
                box1.round(20.f, 3, 200.f);
                box2.round(20.f, 3, 200.f);
                boxleft.rightround(20.f, 2);
                boxright.leftround(20.f, 2);
                stage = 8;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 8 && levelTime / sf::seconds(1.f) > 8.f)
            {
                boxdown.randomfall({ 300.f,150.f }, 2, -pi / 2);
                boxup.randomfall({ 300.f,150.f }, 2, pi / 2);
                boxleft.rightround(20.f, 1);
                boxright.leftround(20.f, 1);
                stage = 9;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 9 && levelTime / sf::seconds(1.f) > 8.f)
            {
                boxdown.randomfall({ 300.f,150.f }, 2, -pi / 2);
                boxup.randomfall({ 300.f,150.f }, 2, pi / 2);
                box1.round(20.f, 3, 200.f);
                box2.round(20.f, 3, 200.f);
                box1.round(20.f, 3, 0);
                box2.round(20.f, 3, 0);
                stage = 10;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 10 && levelTime / sf::seconds(1.f) > 8.f)
            {
                boxup.randomfall({ 300.f,150.f }, 2, pi / 2);
                boxleft.randomfall({ 200.f,140.f }, 2, 0);
                box1.round(20.f, 3, 200.f);
                stage = 11;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 11 && levelTime / sf::seconds(1.f) > 8.f)
            {
                boxdown.randomfall({ 300.f,150.f }, 2, -pi / 2);
                boxright.randomfall({ 200.f,140.f }, 2, pi);
                box2.round(20.f, 3, 200.f);
                stage = 12;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 12 && levelTime / sf::seconds(1.f) > 8.f)
            {
                boxleft.boom(40.f, 1, 0, 2.f, 5);
                boxright.boom(40.f, 1, pi, 2.f, 5);
                stage = 13;
                levelTime = sf::seconds(0.f);
            }
        }
    }
    return 0;
}
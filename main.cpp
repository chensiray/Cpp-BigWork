#include "Button.h"
#include "Player.h"
#include "Bullet.h"
#include "BulletBox.h"
#include "rescource.h"
int main() {
    //窗口
    sf::RenderWindow window(sf::VideoMode({ 1920,1080 }), "True Music", sf::Style::Default);
    window.setFramerateLimit(60);
    sf::View gameview(sf::FloatRect({ 0,0 }, { 1920,1080 }));
    updateViewport(window, gameview);
    window.setVerticalSyncEnabled(true);
    //时间
    sf::Clock gameClock;
    sf::Time levelTime;
    sf::Time lastTime, currentTime;
    bool isPaused = false;
    //关卡阶段
    int level = 0;
    int stage = 0;
    //按钮
    sf::String start = U"选择关卡";
    sf::String backon = U"继续游戏";
    sf::String pause = U"暂停游戏";
    sf::String back = U"返回主页";
    sf::String level1 = U"第一关";
    sf::String level2 = U"第二关";
    sf::String level3 = U"第三关";
    sf::String level4 = U"无限挑战";
    Button startButton(start, { 300.f,120.f }, { 960.f,200.f }, sf::Color::Blue);
    Button levelButton1(level1, { 300.f,120.f }, { 360.f,700.f }, sf::Color::Green);
    Button levelButton2(level2, { 300.f,120.f }, { 760.f,700.f }, sf::Color::Cyan);
    Button levelButton3(level3, { 300.f,120.f }, { 1160.f,700.f }, sf::Color::Magenta);
    Button levelButton4(level4, { 300.f,120.f }, { 1560.f,700.f }, sf::Color::Transparent);
    Button pauseButton(pause, { 200.f,120.f }, { 1750,55.f }, sf::Color::Blue);
    Button backButton(back, { 320.f,100.f }, { 960.f,540.f }, sf::Color::Blue);
    MusicButton musicButton({ 40.f,40.f }, { 1750.f,200.f });
    bool islevel = false;
    //图像
    sf::Sprite award(awardTexture);
    Button failtext(U"闯关失败！您成功撑过了100颗子弹！", {800.f,200.f}, {960.f,700.f}, sf::Color::Transparent);
    Button successtext(U"闯关成功！您成功撑过了100颗子弹！获得分数100分！", {800.f,200.f}, {960.f,700.f}, sf::Color::Transparent);
    award.setOrigin({ 960.f,540.f });
    award.setPosition({ 960.f,300.f });
    award.setScale({ 0.4f,0.4f });
    bool isover = false;
    //玩家
    Player player(0, 100.f, 100.f);
    //子弹
    std::vector<std::unique_ptr<Bullet>> bullets;
    BulletBox box1 = BulletBox({ 480.f,360.f }, bullets);//左上子弹源
    BulletBox box2 = BulletBox({ 1440.f,360.f }, bullets);//右上子弹源
    BulletBox boxleft = BulletBox({ 20.f,540.f }, bullets);//左侧子弹源
    BulletBox boxright = BulletBox({ 1900.f,540.f }, bullets);//右侧子弹源
    BulletBox boxup = BulletBox({ 960.f,20.f }, bullets);//上方子弹源
    BulletBox boxdown = BulletBox({ 960.f,1060.f }, bullets);//下方子弹源
    //音乐
    bool ismusic = true;
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
            if (musicButton.headleEvent(*event, window))
            {
                ismusic = !ismusic;
            }
            if (level == 0)
            {
                if (startButton.headleEvent(*event, window))//开始游戏
                {
                    islevel = !islevel;
                }
                if (islevel)
                {
                    if (levelButton1.headleEvent(*event, window))//开始游戏
                    {
                        musicmain.stop();
                        player.addlife(5);
                        level = 1;
                        levelTime = sf::seconds(0.f);
                    }
                    if (levelButton2.headleEvent(*event, window))//开始游戏
                    {
                        musicmain.stop();
                        player.addlife(3);
                        level = 2;
                        levelTime = sf::seconds(0.f);
                    }
                    if (levelButton3.headleEvent(*event, window))//开始游戏
                    {
                        musicmain.stop();
                        player.addlife(3);
                        level = 3;
                        levelTime = sf::seconds(0.f);
                    }
                    if (levelButton4.headleEvent(*event, window))//开始游戏
                    {
                        musicmain.stop();
                        player.addlife(3);
                        level = 4;
                        levelTime = sf::seconds(0.f);
                    }
                }
            }
            if (level > 0)
            {
                if (!window.hasFocus())
                {
                    isPaused = true;
                    pauseButton.setText(backon);
                }
                if (pauseButton.headleEvent(*event, window))
                {
                    if (isPaused)
                    {
                        pauseButton.setText(pause);
                        isPaused = false;
                    }
                    else
                    {
                        isPaused = true;
                        pauseButton.setText(backon);
                    }
                }
                if (isPaused || isover)
                {
                    if (backButton.headleEvent(*event, window))
                    {
                        isPaused = false;
                        isover = false;
                        pauseButton.setText(pause);
                        player = Player(0, 100.f, 100.f);
                        levelTime = sf::seconds(0.f);
                        level = 0;
                        stage = 0;
                        musiclevel1.stop();
                        musiclevel2.stop();
                        musiclevel3.stop();
                        musiclevel4.stop();
                        Bullet::count = 0;
                        for (auto& bullet : bullets)
                        {
                            bullet->isactive = false;
                        }
                        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), cmp), bullets.end());
                    }
                }
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if (keyPressed->code == sf::Keyboard::Key::P)
                    {
                        if (isPaused)
                        {
                            pauseButton.setText(pause);
                            isPaused = false;
                        }
                        else
                        {
                            isPaused = true;
                            pauseButton.setText(backon);
                        }
                    }
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
        musicButton.update();
        musicButton.draw(window, sf::RenderStates::Default);
        //主菜单
        if (level == 0)
        {
            if (musicmain.getStatus() != sf::Music::Status::Playing)
            {
                musicmain.play();
            }
            if (ismusic)
            {
                musicmain.setVolume(100.f);
            }
            else
            {
                musicmain.setVolume(0.f);
            }
            sf::Vector2u windowSize = window.getSize();
            if (windowSize.x > 0 && windowSize.y > 0)
            {
                sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
                sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);
                startButton.update(mouseWorld);
                if (islevel)
                {
                    levelButton1.update(mouseWorld);
                    levelButton2.update(mouseWorld);
                    levelButton3.update(mouseWorld);
                    levelButton4.update(mouseWorld);
                }
            }
            startButton.draw(window, sf::RenderStates::Default);
            if (islevel)
            {
                levelButton1.draw(window, sf::RenderStates::Default);
                levelButton2.draw(window, sf::RenderStates::Default);
                levelButton3.draw(window, sf::RenderStates::Default);
                levelButton4.draw(window, sf::RenderStates::Default);
            }
        }
        //游戏中
        if (level > 0)
        {
            //暂停
            if (!window.hasFocus())
            {
                isPaused = true;
                pauseButton.setText(backon);
            }
            sf::Vector2u windowSize = window.getSize();
            if (windowSize.x > 0 && windowSize.y > 0)
            {
                sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
                sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);
                pauseButton.update(mouseWorld);
                if (isPaused || !player.alive()) backButton.update(mouseWorld);
            }
            if (isover)
            {
                if (player.alive())
                {
                    sf::String success = U"闯关成功！您成功撑过了" + sf::String(std::to_string(Bullet::count)) + U"颗子弹！获得分数" + sf::String(std::to_string(player.getlife() * 8 + 60)) + U"分！";
                    successtext.setText(success);
                    successtext.draw(window, sf::RenderStates::Default);
                }
                window.draw(award, sf::RenderStates::Default);
                backButton.draw(window, sf::RenderStates::Default);
            }
            if (!player.alive())
            {
                isover = true;
                sf::String fail = U"闯关失败！您成功撑过了" + sf::String(std::to_string(Bullet::count)) + U"颗子弹!";
                failtext.setText(fail);
                failtext.draw(window, sf::RenderStates::Default);
            }
            if (isPaused)
            {
                backButton.draw(window, sf::RenderStates::Default);
            }
            //子弹和玩家更新
            if (!isPaused && !isover)
            {
                player.update();
            }
            pauseButton.draw(window, sf::RenderStates::Default);
            if (!isover)
            {
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
                bullets.erase(std::remove_if(bullets.begin(), bullets.end(), cmp), bullets.end());
            }
            for (auto& bullet : bullets)
            {
                bullet->draw(window, sf::RenderStates::Default);
            }
            player.draw(window, sf::RenderStates::Default);
        }
        window.display();
        //第一关
        if (level == 1)
        {
            if (isover)
            {
                musiclevel1.stop();
                continue;
            }
            if (musiclevel1.getStatus() != sf::Music::Status::Playing && !isPaused)
            {
                musiclevel1.play();
            }
            if (isPaused)
            {
                musiclevel1.pause();
            }
            if (ismusic)
            {
                musiclevel1.setVolume(100.f);
            }
            else
            {
                musiclevel1.setVolume(0.f);
            }
            if (stage == 0 && levelTime / sf::seconds(1.f) > 3.f)
            {
                box1.round(20.f, 2, 200.f);
                box2.round(20.f, 2, 200.f);
                box1.round(20.f, 2, 0);
                box2.round(20.f, 2, 0);
                stage = 1;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 1 && levelTime / sf::seconds(1.f) > 2.f)
            {
                box1.round(20.f, 2, 200.f);
                box2.round(20.f, 2, 200.f);
                box1.round(20.f, 2, 0);
                box2.round(20.f, 2, 0);
                boxleft.follow(40.f, 2, &player);
                boxright.follow(40.f, 2, &player);
                stage = 2;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 2 && levelTime / sf::seconds(1.f) > 2.f)
            {
                boxleft.rightround(20.f, 2);
                box2.follow(40.f, 1,&player);
                boxright.leftround(20.f, 2);
                box1.follow(40.f, 1, &player);
                stage = 3;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 3 && levelTime / sf::seconds(1.f) > 3.f)
            {
                boxup.follow(40.f, 1, &player);
                boxdown.follow(40.f, 1, &player);
                stage = 4;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 4 && levelTime / sf::seconds(1.f) > 2.f)
            {
                boxleft.randomfall({ 200.f,140.f }, 2, 0);
                box2.follow(40.f, 2, &player);
                box2.follow(40.f, 3, &player);
                stage = 5;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 5 && levelTime / sf::seconds(1.f) > 8.f)
            {
                boxright.randomfall({ 200.f,140.f }, 2, pi);
                box1.follow(40.f, 2, &player);
                box1.follow(40.f, 3, &player);
                stage = 6;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 6 && levelTime / sf::seconds(1.f) > 8.f)
            {
                boxleft.boom(40.f, 1, 0, 2.f, 5);
                boxright.boom(40.f, 1, pi, 2.f, 5);
                box1.round(20.f, 3, 100.f);
                box2.round(20.f, 3, 100.f);
                stage = 7;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 7 && levelTime / sf::seconds(1.f) > 8.f)
            {
                boxleft.randomfall({ 200.f,140.f }, 2, 0);
                boxright.randomfall({ 200.f,140.f }, 2, pi);
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
                boxleft.follow(40.f, 2, &player);
                boxright.follow(40.f, 2, &player);
                stage = 10;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 10 && levelTime / sf::seconds(1.f) > 8.f)
            {
                boxup.randomfall({ 300.f,150.f }, 2, pi / 2);
                boxleft.randomfall({ 200.f,140.f }, 2, 0);
                boxdown.randomfall({ 300.f,150.f }, 2, -pi / 2);
                boxright.randomfall({ 200.f,140.f }, 2, pi);
                stage = 11;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 11 && levelTime / sf::seconds(1.f) > 8.f)
            {
                boxup.boom(40.f, 1, pi/2, 2.f, 5);
                boxdown.boom(40.f, 1, -pi/2, 2.f, 5);
                boxright.leftround(20.f, 2);
                boxleft.rightround(20.f, 2);
                stage = 12;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 12 && levelTime / sf::seconds(1.f) > 7.f)
            {
                boxleft.plane({100.f,30.f}, 1, 0, 1.f);
                boxright.plane({100.f,30.f}, 1, pi, 1.f);
                stage = 13;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 13 && levelTime / sf::seconds(1.f) > 4.f)
            {
                boxup.plane({ 30.f,100.f }, 1, pi/2, 1.f);
                boxdown.plane({ 30.f,100.f }, 1, -pi/2, 1.f);
                stage = 14;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 14 && levelTime / sf::seconds(1.f) > 8.f)
            {
                isover = true;
            }
        }
        if (level == 2)
        {
            if (isover)
            {
                musiclevel2.stop();
                continue;
            }
            if (musiclevel2.getStatus() != sf::Music::Status::Playing && !isPaused)
            {
                musiclevel2.play();
            }
            if (isPaused)
            {
                musiclevel2.pause();
            }
            if (ismusic)
            {
                musiclevel2.setVolume(100.f);
            }
            else
            {
                musiclevel2.setVolume(0.f);
            }
        }
    }
    return 0;
}
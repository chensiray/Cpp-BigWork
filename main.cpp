#include "Button.h"
#include "Player.h"
#include "Bullet.h"
#include "BulletBox.h"
#include "ParticleSystem.h"
#include "Lottery.h"
#include "Item.h"
#include "rescource.h"
sf::String Back = U"继续游戏";
sf::String Bingo = U"点击抽奖";
static bool cmp1(const std::unique_ptr<Bullet>& bullet)
{
    return !bullet->isactive;
}
static bool cmp2(const std::unique_ptr<ParticleSystem>& particle)
{
    return !particle->isactive;
}
static bool cmp3(const std::unique_ptr<Item>& item)
{
    return !item->isactive;
}
int main() {
    //窗口
    sf::RenderWindow window(sf::VideoMode({ 1920,1080 }), "FrostGap", sf::Style::Default);
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
    int stage = 16;
    //按钮
    sf::String start = U"选择关卡";
    sf::String explain = U"游戏说明";
    sf::String exit = U"退出游戏";
    sf::String backon = U"继续游戏";
    sf::String pause = U"暂停游戏";
    sf::String back = U"返回主页";
    sf::String level1 = U"第一关";
    sf::String level2 = U"第二关";
    sf::String level3 = U"第三关";
    sf::String level4 = U"无限挑战";
    Button startButton(start, { 300.f,120.f }, { 960.f,400.f }, sf::Color::Blue);
    Button explainButton(explain, { 300.f,120.f }, { 960.f,640.f }, sf::Color::Blue);
    Button exitButton(exit, { 300.f,120.f }, { 960.f,880.f }, sf::Color::Blue);
    Button levelButton1(level1, { 300.f,120.f }, { 400.f,400.f }, sf::Color::Green);
    Button levelButton2(level2, { 300.f,120.f }, { 400.f,700.f }, sf::Color::Cyan);
    Button levelButton3(level3, { 300.f,120.f }, { 1520.f,400.f }, sf::Color::Magenta);
    Button levelButton4(level4, { 300.f,120.f }, { 1520.f,700.f }, sf::Color::Transparent);
    Button pauseButton(pause, { 200.f,120.f }, { 1750,55.f }, sf::Color::Blue);
    Button backButton(back, { 320.f,100.f }, { 960.f,540.f }, sf::Color::Blue);
    MusicButton musicButton({ 40.f,40.f }, { 1750.f,200.f });
    bool islevel = false;
    //图像
    sf::String cointext = U"消耗1个硬币开始抽奖，你有0个硬币";
    sf::String failString = U"下次一定...";
    sf::String lotteryStrings[5] = { U"你抽中了红心，将获得1点生命值", U"你抽中了火箭，将提升40%的速度", U"你抽中了护盾，按E使用获得3秒无敌", U"你抽中了时钟，按Q使用使子弹暂停4秒", U"你抽中了扫把，按R使用清除全图子弹" };
    sf::Sprite award(awardTexture);
    Button failtext(U"闯关失败！您成功撑过了100颗子弹！", {800.f,200.f}, {960.f,700.f}, sf::Color::Transparent);
    Button successtext(U"闯关成功！您成功撑过了100颗子弹！获得分数100分！", {800.f,200.f}, {960.f,700.f}, sf::Color::Transparent);
    Button lotterytext(cointext, {1000.f,200.f}, {960.f,100.f}, sf::Color::Transparent);
    award.setOrigin({ 960.f,540.f });
    award.setPosition({ 960.f,300.f });
    award.setScale({ 0.4f,0.4f });
    bool isover = false;
    bool iscoin = true;
    bool isclock = false;
    sf::Time clockBirthTime;
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
    //粒子特效
    std::vector<std::unique_ptr<ParticleSystem>> particles;
    //掉落物
    std::vector<std::unique_ptr<Item>> items;
    //音乐
    bool ismusic = true;
    //抽奖机
    bool islottery = false;
    Lottery lottery({ 960.f,300.f });
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
                if (explainButton.headleEvent(*event, window))//游戏说明
                {
                    
                }
                if (exitButton.headleEvent(*event, window))//退出游戏
                {
                    std::cout << "游戏时间:" << gameTime / sf::seconds(1.f) << "秒" << std::endl;
                    window.close();
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
                        player.addlife(4);
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
                if (islottery)
                {
                    int id = lottery.headleEvent(*event, window);
                    if (id == 1)
                    {
                        if (player.havecoin >= 1)
                        {
                            player.havecoin -= 1;
                            lottery.getPrize();
                        }
                    }
                    else if (id == 2)
                    {
                        islottery = false;
                    }
                }
                if (isPaused || isover)
                {
                    if (backButton.headleEvent(*event, window))
                    {
                        isPaused = false;
                        isover = false;
                        islottery = false;
                        iscoin = true;
                        isclock = false;
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
                        for (auto& particle : particles)
                        {
                            particle->isactive = false;
                        }
                        for (auto& item : items)
                        {
                            item->isactive = false;
                        }
                        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), cmp1), bullets.end());
                        particles.erase(std::remove_if(particles.begin(), particles.end(), cmp2), particles.end());
                        items.erase(std::remove_if(items.begin(), items.end(), cmp3), items.end());
                    }
                }
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if (keyPressed->code == sf::Keyboard::Key::P)//暂停
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
                    if (keyPressed->code == sf::Keyboard::Key::E)//护盾
                    {
                        if (player.haveshield >= 1)
                        {
                            player.haveshield--;
                            player.shield();
                        }
                    }
                    if (keyPressed->code == sf::Keyboard::Key::Q)//时钟
                    {
                        if (player.haveclock >= 1)
                        {
                            player.haveclock--;
                            isclock = true;
                            clockBirthTime = currentTime;
                        }
                    }
                    if (keyPressed->code == sf::Keyboard::Key::R)//清除
                    {
                        if (player.haveclear >= 1)
                        {
                            player.haveclear--;
                            particles.push_back(std::unique_ptr<ParticleSystem>(new ParticleSystem({ 960.f,540.f }, sf::Color::Yellow, 10000, 1.f, 1000.f)));
                            for (auto& bullet : bullets)
                            {
                                bullet->isactive = false;
                            }
                            bullets.erase(std::remove_if(bullets.begin(), bullets.end(), cmp1), bullets.end());
                        }
                    }
                }
            }
        }
        //更新循环
        window.setView(gameview);
        window.clear(sf::Color::Black);
        currentTime = gameClock.getElapsedTime();
        if (isclock && currentTime - clockBirthTime > sf::seconds(4.f))
        {
            isclock = false;
        }
        if (!isPaused && !isclock)
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
                explainButton.update(mouseWorld);
                exitButton.update(mouseWorld);
                if (islevel)
                {
                    levelButton1.update(mouseWorld);
                    levelButton2.update(mouseWorld);
                    levelButton3.update(mouseWorld);
                    levelButton4.update(mouseWorld);
                }
            }
            startButton.draw(window, sf::RenderStates::Default);
            exitButton.draw(window, sf::RenderStates::Default);
            explainButton.draw(window, sf::RenderStates::Default);
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
            //暂停结算抽奖
            cointext = U"消耗1个硬币开始抽奖，你有" + sf::String(std::to_string(player.havecoin)) + U"个硬币";
            if (iscoin)
            {
                lotterytext.setText(cointext);
            }
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
                if (islottery)
                {
                    if (lottery.update(mouseWorld))
                    {
                        iscoin = false;
                        int code = lottery.getDisplay();
                        int ty = code / 10, tot = code % 10;
                        if (tot == 1)
                        {
                            items.push_back(std::unique_ptr<Item>(new Item({ 960.f,500.f }, ty, 10.f)));
                            particles.push_back(std::unique_ptr<ParticleSystem>(new ParticleSystem({ 710.f,300.f }, sf::Color::Yellow, 1000, 2.0f, 100.f)));
                            particles.push_back(std::unique_ptr<ParticleSystem>(new ParticleSystem({ 960.f,300.f }, sf::Color::Yellow, 1000, 2.0f, 100.f)));
                            particles.push_back(std::unique_ptr<ParticleSystem>(new ParticleSystem({ 1210.f,300.f }, sf::Color::Yellow, 1000, 2.0f, 100.f)));
                        }
                        else if (tot == 2)
                        {
                            items.push_back(std::unique_ptr<Item>(new Item({ 860.f,500.f }, ty, 10.f)));
                            items.push_back(std::unique_ptr<Item>(new Item({ 960.f,500.f }, ty, 10.f)));
                            items.push_back(std::unique_ptr<Item>(new Item({ 1060.f,500.f }, ty, 10.f)));
                            particles.push_back(std::unique_ptr<ParticleSystem>(new ParticleSystem({ 960.f,300.f }, sf::Color::Yellow, 5000, 5.f, 800.f)));
                        }
                        if (tot != 0)
                        {
                            lotterytext.setText(lotteryStrings[ty]);
                        }
                        else
                        {
                            lotterytext.setText(failString);
                        }
                    }
                }
            }
            if (islottery)
            {
                lottery.draw(window, sf::RenderStates::Default);
                lotterytext.draw(window, sf::RenderStates::Default);
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
                    if (!isPaused && !isclock)
                    {
                        bullet->update();
                    }
                }
                for (size_t i = 0; i < particles.size(); i++)
                {
                    auto& particle = particles[i];
                    if (!isPaused && !isclock)
                    {
                        particle->update();
                    }
                    particle->draw(window, sf::RenderStates::Default);
                }
                for (size_t i = 0; i < items.size(); i++)
                {
                    auto& item = items[i];
                    if (!isPaused && !isclock)
                    {
                        item->update();
                        item->checkCollision(player);
                    }
                    item->draw(window, sf::RenderStates::Default);
                }
                if (!isPaused && !isclock)
                {
                    for (auto& bullet : bullets)
                    {
                        if (bullet->checkCollision(player.getBounds()))
                        {
                            bullet->isactive = false;
                            particles.push_back(std::unique_ptr<ParticleSystem>(new ParticleSystem(player.getPosition(), sf::Color::Red, 600, 0.6f, 70.f)));
                            player.hurt();
                        }
                    }
                }
                bullets.erase(std::remove_if(bullets.begin(), bullets.end(), cmp1), bullets.end());
                particles.erase(std::remove_if(particles.begin(), particles.end(), cmp2), particles.end());
                items.erase(std::remove_if(items.begin(), items.end(), cmp3), items.end());
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
                boxleft.sector(20.f, 2, 0);
                box2.follow(40.f, 1,&player);
                boxright.sector(20.f, 2, pi);
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
                boxleft.sector(20.f, 2, 0);
                boxright.sector(20.f, 2, pi);
                stage = 8;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 8 && levelTime / sf::seconds(1.f) > 8.f)
            {
                boxdown.randomfall({ 300.f,150.f }, 2, -pi / 2);
                boxup.randomfall({ 300.f,150.f }, 2, pi / 2);
                boxleft.sector(20.f, 1, 0);
                boxright.sector(20.f, 1, pi);
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
                boxright.sector(20.f, 2, pi);
                boxleft.sector(20.f, 2, 0);
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
            if (musiclevel2.getStatus() != sf::Music::Status::Playing && !isPaused && !islottery && !isclock)
            {
                musiclevel2.play();
            }
            if (isPaused || islottery || isclock)
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
            if (stage == 0 && levelTime / sf::seconds(1.f) > 3.f)
            {
                box1.plane({ 30.f,100.f }, 3, pi / 2, 0.5f);
                box2.plane({ 30.f,100.f }, 3, pi / 2, 0.5f);
                boxdown.plane({ 30.f,100.f }, 3, -pi / 2, 0.5f);
                stage = 1;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 1 && levelTime / sf::seconds(1.f) > 6.f)
            {
                boxup.boom(40.f, 3, pi / 2, 1.f, 3);
                boxdown.boom(40.f, 3, -pi / 2, 1.f, 3);
                stage = 2;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 2 && levelTime / sf::seconds(1.f) > 5.f)
            {
                boxdown.randomfall({ 300.f,150.f }, 2, -pi / 2);
                boxup.randomfall({ 300.f,150.f }, 2, pi / 2);
                boxright.boom(60.f, 3, pi, 1.f, 3);
                boxleft.boom(60.f, 3, 0, 1.f, 3);
                items.push_back(std::unique_ptr<Item>(new Item({ 960.f,540.f }, 5, 6.f)));
                stage = 3;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 3 && levelTime / sf::seconds(1.f) > 4.f)
            {
                boxright.follow(60.f, 3,&player);
                boxleft.follow(60.f, 3,&player);
                boxup.follow(60.f, 3,&player);
                boxdown.follow(60.f, 3,&player);
                stage = 4;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 4 && levelTime / sf::seconds(1.f) > 4.f)
            {
                items.push_back(std::unique_ptr<Item>(new Item({ 1400.f,500.f }, 5, 6.f)));
                islottery = true;
                iscoin = true;
                stage = 5;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 5 && !islottery)
            {
                stage = 6;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 6 && levelTime / sf::seconds(1.f) > 3.f)
            {
                boxleft.randomfall({ 200.f,140.f }, 2, 0);
                boxright.boom(40.f, 2, pi, 2.f, 3);
                boxup.plane({ 30.f,100.f }, 1, pi / 2, 0.7f);
                stage = 7;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 7 && levelTime / sf::seconds(1.f) > 3.f)
            {
                boxright.sector(20.f, 2, pi);
                boxleft.sector(20.f, 2, 0);
                boxdown.plane({ 30.f,100.f }, 1, -pi / 2, 0.5f);
                box1.round(20.f, 2, -50.f);
                stage = 8;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 8 && levelTime / sf::seconds(1.f) > 2.f)
            {
                boxup.randomfall({ 300.f,150.f }, 2, pi / 2);
                boxdown.round(20.f, 2, 0);
                boxleft.follow(40.f, 1, &player);
                box2.round(20.f, 2, -50.f);
                stage = 9;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 9 && levelTime / sf::seconds(1.f) > 3.f)
            {
                box1.boom(40.f, 2, pi/4, 1.5f, 3);
                box2.boom(40.f, 2, pi*3/4, 1.5f, 3);
                stage = 10;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 10 && levelTime / sf::seconds(1.f) > 2.f)
            {
                boxup.plane({ 30.f,100.f }, 2, pi / 2, 0.5f);
                boxdown.plane({ 30.f,100.f }, 2, -pi / 2, 0.5f);
                boxright.follow(40.f, 2, &player);
                stage = 11;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 11 && levelTime / sf::seconds(1.f) > 5.f)
            {
                items.push_back(std::unique_ptr<Item>(new Item({ 960.f,540.f }, 5, 6.f)));
                box1.round(20.f, 2, 10.f);
                box2.round(20.f, 2, 10.f);
                stage = 12;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 12 && levelTime / sf::seconds(1.f) > 4.f)
            {
              
                boxright.sector(20.f, 2, pi);
                boxleft.sector(20.f, 2, 0);
                boxdown.follow(30.f, 3, &player);
                stage = 13;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 13 && levelTime / sf::seconds(1.f) > 5.f)
            {
                items.push_back(std::unique_ptr<Item>(new Item({ 960.f,540.f }, 5, 6.f)));
                boxright.boom(40.f, 2, pi, 1.f, 5);
                boxleft.boom(40.f, 2, 0, 1.f, 5);
                stage = 14;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 14 && levelTime / sf::seconds(1.f) > 5.f)
            {
                items.push_back(std::unique_ptr<Item>(new Item({ 1400.f,500.f }, 5, 6.f)));
                islottery = true;
                iscoin = true;
                stage = 15;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 15 && !islottery)
            {
                stage = 16;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 16 && levelTime / sf::seconds(1.f) > 3.f)
            {
                boxright.boom(40.f, 2, pi, 1.f, 3);
                boxleft.boom(40.f, 2, 0, 1.f, 3);
                boxup.sector(20.f, 2, pi / 2);
                boxdown.sector(20.f, 2, -pi / 2);
                stage = 17;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 17 && levelTime / sf::seconds(1.f) > 3.f)
            {
                boxup.boom(40.f, 2, pi / 2, 1.f, 5);
                boxdown.boom(40.f, 2, -pi / 2, 1.f, 5);
                boxup.sector(20.f, 2, pi / 2);
                boxdown.sector(20.f, 2, -pi / 2);
                stage = 18;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 18 && levelTime / sf::seconds(1.f) > 3.f)
            {
                boxleft.plane({ 100.f,30.f }, 1, 0, 0.7f);
                boxright.plane({ 100.f,30.f }, 1, pi, 0.7f);
                box1.sector(20.f, 2, pi / 2);
                box2.sector(20.f, 2, pi / 2);
                stage = 19;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 19 && levelTime / sf::seconds(1.f) > 2.f)
            {
                boxleft.sector(20.f, 2, 0);
                boxright.sector(20.f, 2, pi);
                stage = 20;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 20 && levelTime / sf::seconds(1.f) > 2.f)
            {
                boxup.sector(20.f, 2, pi / 2);
                boxdown.sector(20.f, 2, -pi / 2);
                stage = 21;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 21 && levelTime / sf::seconds(1.f) > 2.f)
            {
                boxleft.boom(40.f, 3, 0, 1.f, 3);
                boxright.boom(40.f, 3, pi, 1.f, 3);
                stage = 22;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 22 && levelTime / sf::seconds(1.f) > 2.f)
            {
                boxup.boom(40.f, 3, pi / 2, 1.f, 5);
                boxdown.boom(40.f, 3, -pi / 2, 1.f, 5);
                stage = 23;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 23 && levelTime / sf::seconds(1.f) > 4.f)
            {
                boxup.follow(40.f, 3, &player);
                boxdown.follow(40.f, 3, &player);
                boxright.follow(40.f, 3, &player);
                boxleft.follow(40.f, 3, &player);
                stage = 24;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 24 && levelTime / sf::seconds(1.f) > 3.f)
            {
                boxdown.sector(20.f, 2, -pi / 2);
                stage = 25;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 25 && levelTime / sf::seconds(1.f) > 2.f)
            {
                box1.round(20.f, 2, 0);
                box2.round(20.f, 2, 0);
                stage = 26;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 26 && levelTime / sf::seconds(1.f) > 6.f)
            {
                isover = true;
            }
        }
        if (level == 3)
        {
            if (isover)
            {
                musiclevel3.stop();
                continue;
            }
            if (musiclevel3.getStatus() != sf::Music::Status::Playing && !isPaused && !islottery && !isclock)
            {
                musiclevel3.play();
            }
            if (isPaused || islottery || isclock)
            {
                musiclevel3.pause();
            }
            if (ismusic)
            {
                musiclevel3.setVolume(100.f);
            }
            else
            {
                musiclevel3.setVolume(0.f);
            }
        }
    }
    return 0;
}
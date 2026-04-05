#include "Button.h"
#include "Player.h"
#include "Bullet.h"
#include "BulletBox.h"
#include "ParticleSystem.h"
#include "Lottery.h"
#include "Item.h"
#include "resource.h"
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
static void updateViewport(const sf::RenderWindow& window, sf::View& gameview)
{
    sf::Vector2u windowSize = window.getSize();
    if (windowSize.x == 0 || windowSize.y == 0) return;
    float windowRatio = static_cast<float>(windowSize.x) / windowSize.y;
    float viewRatio = 1920.f / 1080.f;
    sf::FloatRect viewport;
    if (windowRatio > viewRatio)
    {
        viewport.size.x = viewRatio / windowRatio;
        viewport.size.y = 1.0f;
    }
    else
    {
        viewport.size.x = 1.0f;
        viewport.size.y = windowRatio / viewRatio;
    }
    viewport.position.x = (1.0f - viewport.size.x) / 2.0f;
    viewport.position.y = (1.0f - viewport.size.y) / 2.0f;
    gameview.setViewport(viewport);
}
int main() {
    //加载资源
    if (!loadResources())
    {
        std::cerr << "游戏资源加载失败" << std::endl;
        std::cout << "按Enter键退出";
        std::cin.get();
        return 0;
    }
    //窗口
    static sf::RenderWindow window(sf::VideoMode({ 1920,1080 }), "FrostGap", sf::Style::Default);
    window.setFramerateLimit(60);
    static sf::View gameview(sf::FloatRect({ 0,0 }, { 1920,1080 }));
    updateViewport(window, gameview);
    //背景
    static sf::VertexArray gradient(sf::PrimitiveType::TriangleFan, 4);
    gradient[0].position = { 0.f,0.f };
    gradient[1].position = { 1920.f,0.f };
    gradient[2].position = { 1920.f,1080.f };
    gradient[3].position = { 0.f,1080.f };
    gradient[0].color = sf::Color({ 0,0,0 });
    gradient[1].color = sf::Color({ 0,0,0 });
    gradient[2].color = sf::Color({ 30,20,50 });
    gradient[3].color = sf::Color({ 30,20,50 });
    //过渡
    static sf::RectangleShape transition({ 1920.f,1080.f });
    transition.setFillColor({ 255,255,255,0 });
    //时间
    static sf::Clock gameClock;
    static sf::Time levelTime;
    static sf::Time lastTime, currentTime;
    static sf::Time shakeTime;
    static sf::Time transitionTime;
    static sf::Time clockBirthTime;
    //关卡阶段
    static int level = 0;
    static int stage = 0;
    static int degree = 0;
    //按钮
    static sf::String backon = U"继续游戏";
    static sf::String pause = U"暂停游戏";
    static sf::String failString = U"下次一定...";
    static sf::String lotteryStrings[5] = { U"你抽中了红心，将获得1点生命值", U"你抽中了火箭，将提升20%的速度", U"你抽中了护盾，按E使用获得3秒无敌", U"你抽中了时钟，按Q使用使子弹暂停4秒", U"你抽中了扫把，按R使用清除全图子弹" };
    static Button startButton(U"选择关卡", { 300.f,100.f }, { 960.f,400.f }, sf::Color::Blue);
    static Button explainButton(U"游戏说明", { 300.f,100.f }, { 960.f,640.f }, sf::Color::Blue);
    static Button exitButton(U"退出游戏", { 300.f,100.f }, { 960.f,880.f }, sf::Color::Blue);
    static Button levelButton1(U"第一关", { 200.f,120.f }, { 400.f,400.f }, sf::Color::Green);
    static Button levelButton2(U"第二关", { 200.f,120.f }, { 400.f,700.f }, sf::Color::Cyan);
    static Button levelButton3(U"第三关", { 200.f,120.f }, { 1520.f,400.f }, sf::Color::Magenta);
    static Button levelButton4(U"无限挑战", { 200.f,120.f }, { 1520.f,700.f }, sf::Color{ 0,0,0,255 });
    static Button pauseButton(pause, { 200.f,120.f }, { 1750,55.f }, sf::Color::Blue);
    static Button backButton(U"返回主页", { 320.f,100.f }, { 960.f,540.f }, sf::Color::Blue);
    static Button nextButton(U"继续挑战", { 320.f,100.f }, { 560.f,850.f }, sf::Color::Red);
    static Button lotteryButton(U"进入抽奖", { 320.f,100.f }, { 1360.f,850.f }, sf::Color::Blue);
    static MusicButton musicButton({ 40.f,40.f }, { 1750.f,200.f });
    //图像文字
    static sf::Text boss(font, U"boss即将出现，带着三颗红心去挑战boss吧", 50);
    boss.setPosition({ 500.f,200.f });
    boss.setFillColor(sf::Color::Red);
    static sf::Text explanation1(font, U"WASD键移动躲避\nP键暂停游戏\nE键使用护盾\nQ键使用时钟\nR键使用清除", 50);
    static sf::Text explanation2(font, U"Music Used:\nMeun:\"Mesmerizing Galaxy \" Kevin MacLeod\nLevel1:\"Obliteration\" Kevin MacLeod\nLevel2:\"Darkling\" Kevin MacLeod\nLevel3:\"Galactic Rap\" Kevin MacLeod\nLevel4:\"Screen Saver\" Kevin MacLeod\n(incompetech.com)\nLicensed under Creative Commons : By Attribution 4.0 License\nhttp://creativecommons.org/licenses/by/4.0/", 20);
    static sf::Text explanation3(font, U"Project Links:\nhttps://github.com/chensiray/FrostGap\nhttps://gitee.com/chensiray/cpp-big-work", 20);
    explanation1.setPosition({ 200.f, 500.f });
    explanation1.setFillColor(sf::Color({ 80,100,220 }));
    explanation2.setPosition({ 1300.f, 700.f });
    explanation3.setPosition({ 1300.f, 400.f });
    static Button failtext(U"闯关失败！您成功撑过了100颗子弹！", { 800.f,200.f }, { 960.f,700.f }, sf::Color::Transparent);
    static Button successtext(U"闯关成功！您成功撑过了100颗子弹！获得分数100分！", { 800.f,200.f }, { 960.f,700.f }, sf::Color::Transparent);
    static Button gaptext(U"无限挑战：您已完成1轮！", { 800.f,200.f }, { 960.f,50.f }, sf::Color::Transparent);
    static Button lotterytext(U"消耗1个硬币开始抽奖，你有0个硬币", { 1000.f,200.f }, { 960.f,100.f }, sf::Color::Transparent);
    static sf::Sprite award(awardTexture);
    award.setOrigin({ 960.f,540.f });
    award.setPosition({ 960.f,300.f });
    award.setScale({ 0.4f,0.4f });
    static sf::Sprite title(titleTexture);
    title.setPosition({ 0.f,0.f });
    //属性
    static bool islevel = false;
    static bool isexplain = false;
    static bool isover = false;
    static bool isshake = false;
    static bool isPaused = false;
    static bool iscoin = true;
    static bool isclock = false;
    static bool isboss = false;
    static bool isgap = false;
    static bool isTransition = false;
    static bool ismusic = true;
    static bool islottery = false;
    //玩家
    Player player(0, 100.f, 100.f);
    //子弹
    static std::vector<std::unique_ptr<Bullet>> bullets;
    static BulletBox box1 = BulletBox({ 480.f,360.f }, bullets);//左上子弹源
    static BulletBox box2 = BulletBox({ 1440.f,360.f }, bullets);//右上子弹源
    static BulletBox boxleft = BulletBox({ 20.f,540.f }, bullets);//左侧子弹源
    static BulletBox boxright = BulletBox({ 1900.f,540.f }, bullets);//右侧子弹源
    static BulletBox boxup = BulletBox({ 960.f,20.f }, bullets);//上方子弹源
    static BulletBox boxdown = BulletBox({ 960.f,1060.f }, bullets);//下方子弹源
    static BulletBox boxboss = BulletBox({ 960.f,400.f }, bullets);//boss子弹源
    //粒子特效
    static std::vector<std::unique_ptr<ParticleSystem>> particles;
    particles.push_back(std::unique_ptr<ParticleSystem>(new ParticleSystem({ 960.f,540.f }, sf::Color({ 255,255,255,80 }), 120, 1000000.f, 1000.f)));
    static ParticleSystem stars({ 960.f,540.f }, sf::Color({ 255,255,255,80 }), 50, 1000000.f, 1000.f);
    //掉落物
    static std::vector<std::unique_ptr<Item>> items;
    //抽奖机
    static Lottery lottery({ 960.f,300.f });
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
                    if (isexplain && islevel) isexplain = false;
                }
                if (explainButton.headleEvent(*event, window))//游戏说明
                {
                    isexplain = !isexplain;
                    if (isexplain && islevel) islevel = false;
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
                        isTransition = true;
                        transitionTime = gameTime;
                        musicmain.stop();
                        player.addlife(5);
                        level = 1;
                        levelTime = sf::seconds(0.f);
                    }
                    if (levelButton2.headleEvent(*event, window))//开始游戏
                    {
                        isTransition = true;
                        transitionTime = gameTime;
                        musicmain.stop();
                        player.addlife(4);
                        level = 2;
                        levelTime = sf::seconds(0.f);
                    }
                    if (levelButton3.headleEvent(*event, window))//开始游戏
                    {
                        isTransition = true;
                        transitionTime = gameTime;
                        musicmain.stop();
                        player.addlife(3);
                        level = 3;
                        levelTime = sf::seconds(0.f);
                    }
                    if (levelButton4.headleEvent(*event, window))//开始游戏
                    {
                        isTransition = true;
                        transitionTime = gameTime;
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
                        isgap = false;
                        levelTime = sf::seconds(0.f);
                    }
                }
                if (isgap&&!islottery)
                {
                    if (nextButton.headleEvent(*event, window))
                    {
                        islottery = false;
                        isgap = false;
                        levelTime = sf::seconds(0.f);
                    }
                    if (lotteryButton.headleEvent(*event, window))
                    {
                        isTransition = true;
                        transitionTime = gameTime;
                        islottery = true;
                    }
                }
                if (isPaused || isover || (isgap && !islottery))
                {
                    if (backButton.headleEvent(*event, window))
                    {
                        isPaused = false;
                        isover = false;
                        islottery = false;
                        iscoin = true;
                        isclock = false;
                        isboss = false;
                        isgap = false;
                        pauseButton.setText(pause);
                        player = Player(0, 100.f, 100.f);
                        levelTime = sf::seconds(0.f);
                        level = 0;
                        stage = 0;
                        degree = 0;
                        musiclevel1.stop();
                        musiclevel2.stop();
                        musiclevel3.stop();
                        musiclevel4.stop();
                        Bullet::count = 0;
                        isTransition = true;
                        transitionTime = gameTime;
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
                        particles.push_back(std::unique_ptr<ParticleSystem>(new ParticleSystem({ 960.f,540.f }, sf::Color({ 255,255,255,80 }), 120, 1000000.f, 1000.f)));
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
                            clearsound.play();
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
        //屏幕与时间更新
        if (isshake)
        {
            sf::View shakeView = gameview;
            shakeView.move({ std::uniform_real_distribution<float>(-6,6)(gen), std::uniform_real_distribution<float>(-6,6)(gen) });
            window.setView(shakeView);
        }
        else
        {
            window.setView(gameview);
        }
        window.clear(sf::Color::Black);
        window.draw(gradient);
        if (isTransition)
        {
            transition.setFillColor(sf::Color({ 255,255,255,static_cast<std::uint8_t>(sin((gameTime - transitionTime) * pi / sf::seconds(1.f)) * 255) }));
            if (gameTime - transitionTime > sf::seconds(1.f))
            {
                transition.setFillColor(sf::Color({ 255,255,255,0 }));
                isTransition = false;
            }
        }
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
        if (isshake && gameTime - shakeTime > sf::seconds(0.1f))
        {
            isshake = false;
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
                musicmain.setVolume(20.f);
            }
            else
            {
                musicmain.setVolume(0.f);
            }
            sf::Vector2u windowSize = window.getSize();
            stars.update();
            stars.draw(window, sf::RenderStates::Default);
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
            if (isexplain)
            {
                window.draw(explanation1);
                window.draw(explanation2);
                window.draw(explanation3);
            }
            window.draw(title);
        }
        //游戏中
        if (level > 0)
        {
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
                            isshake = true;
                            shakeTime = gameTime;
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
            //暂停结算抽奖
            sf::String cointext = U"消耗1个硬币开始抽奖，你有" + sf::String(std::to_string(player.havecoin)) + U"个硬币";
            if (iscoin)
            {
                lotterytext.setText(cointext);
            }
            if (isboss)
            {
                window.draw(boss);
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
                if (isPaused || isover || (isgap && !islottery)) backButton.update(mouseWorld);
                if (isgap && !islottery)
                {
                    lotteryButton.update(mouseWorld);
                    nextButton.update(mouseWorld);
                }
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
                            lotterysound.play();
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
            if (isover || (isgap && !islottery))
            {
                if (player.alive())
                {
                    sf::String success = U"闯关成功！您成功撑过了" + sf::String(std::to_string(Bullet::count)) + U"颗子弹！获得分数" + sf::String(std::to_string(Bullet::count / 10 + player.getlife() * 8)) + U"分！";
                    successtext.setText(success);
                    successtext.draw(window, sf::RenderStates::Default);
                }
                window.draw(award, sf::RenderStates::Default);
                backButton.draw(window, sf::RenderStates::Default);
                if (!isgap && level == 4)
                {
                    sf::String gapString = U"无限挑战：您已达到" + sf::String(std::to_string(degree)) + U"轮！";
                    gaptext.setText(gapString);
                    window.draw(gaptext);
                }
                if (isgap)
                {
                    sf::String gapString = U"无限挑战：您已完成" + sf::String(std::to_string(degree)) + U"轮！";
                    gaptext.setText(gapString);
                    window.draw(gaptext);
                    lotteryButton.draw(window, sf::RenderStates::Default);
                    nextButton.draw(window, sf::RenderStates::Default);
                }
            }
            if (!player.alive())
            {
                isover = true;
                sf::String fail = U"闯关失败！您撑过了" + sf::String(std::to_string(Bullet::count)) + U"颗子弹!";
                failtext.setText(fail);
                failtext.draw(window, sf::RenderStates::Default);
            }
            if (isPaused)
            {
                backButton.draw(window, sf::RenderStates::Default);
            }
            player.draw(window, sf::RenderStates::Default);
        }
        //渲染
        if (isTransition && gameTime - transitionTime < sf::seconds(0.5f))
        {
            window.draw(gradient);
        }
        window.draw(transition);
        window.display();
        //关卡流程
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
                musiclevel1.setVolume(20.f);
            }
            else
            {
                musiclevel1.setVolume(0.f);
            }
            if (stage == 0 && levelTime / sf::seconds(1.f) > 3.f)
            {
                box1.round(10.f, 2, 200.f);
                box2.round(10.f, 2, 200.f);
                box1.round(10.f, 2, 0);
                box2.round(10.f, 2, 0);
                stage = 1;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 1 && levelTime / sf::seconds(1.f) > 2.f)
            {
                box1.round(10.f, 2, 200.f);
                box2.round(10.f, 2, 200.f);
                box1.round(10.f, 2, 0);
                box2.round(10.f, 2, 0);
                boxleft.follow(40.f, 2, &player);
                boxright.follow(40.f, 2, &player);
                stage = 2;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 2 && levelTime / sf::seconds(1.f) > 2.f)
            {
                boxleft.sector(10.f, 2, 0);
                box2.follow(40.f, 1,&player);
                boxright.sector(10.f, 2, pi);
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
                box1.round(10.f, 3, 100.f);
                box2.round(10.f, 3, 100.f);
                stage = 7;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 7 && levelTime / sf::seconds(1.f) > 8.f)
            {
                boxleft.randomfall({ 200.f,140.f }, 2, 0);
                boxright.randomfall({ 200.f,140.f }, 2, pi);
                boxleft.sector(10.f, 2, 0);
                boxright.sector(10.f, 2, pi);
                stage = 8;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 8 && levelTime / sf::seconds(1.f) > 8.f)
            {
                boxdown.randomfall({ 300.f,150.f }, 2, -pi / 2);
                boxup.randomfall({ 300.f,150.f }, 2, pi / 2);
                boxleft.sector(10.f, 1, 0);
                boxright.sector(10.f, 1, pi);
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
                boxright.sector(10.f, 2, pi);
                boxleft.sector(10.f, 2, 0);
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
                musiclevel2.setVolume(20.f);
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
                boxright.sector(10.f, 2, pi);
                boxleft.sector(10.f, 2, 0);
                boxdown.plane({ 30.f,100.f }, 1, -pi / 2, 0.5f);
                box1.round(10.f, 2, -50.f);
                stage = 8;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 8 && levelTime / sf::seconds(1.f) > 2.f)
            {
                boxup.randomfall({ 300.f,150.f }, 2, pi / 2);
                boxdown.round(10.f, 2, 0);
                boxleft.follow(40.f, 1, &player);
                box2.round(10.f, 2, -50.f);
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
                box1.round(10.f, 2, 10.f);
                box2.round(10.f, 2, 10.f);
                stage = 12;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 12 && levelTime / sf::seconds(1.f) > 4.f)
            {
              
                boxright.sector(10.f, 2, pi);
                boxleft.sector(10.f, 2, 0);
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
                boxup.sector(10.f, 2, pi / 2);
                boxdown.sector(10.f, 2, -pi / 2);
                stage = 17;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 17 && levelTime / sf::seconds(1.f) > 3.f)
            {
                boxup.boom(40.f, 2, pi / 2, 1.f, 5);
                boxdown.boom(40.f, 2, -pi / 2, 1.f, 5);
                boxup.sector(10.f, 2, pi / 2);
                boxdown.sector(10.f, 2, -pi / 2);
                stage = 18;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 18 && levelTime / sf::seconds(1.f) > 3.f)
            {
                boxleft.plane({ 100.f,30.f }, 1, 0, 0.7f);
                boxright.plane({ 100.f,30.f }, 1, pi, 0.7f);
                box1.sector(10.f, 2, pi / 2);
                box2.sector(10.f, 2, pi / 2);
                stage = 19;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 19 && levelTime / sf::seconds(1.f) > 2.f)
            {
                boxleft.sector(10.f, 2, 0);
                boxright.sector(10.f, 2, pi);
                stage = 20;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 20 && levelTime / sf::seconds(1.f) > 2.f)
            {
                boxup.sector(10.f, 2, pi / 2);
                boxdown.sector(10.f, 2, -pi / 2);
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
                boxdown.sector(10.f, 2, -pi / 2);
                stage = 25;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 25 && levelTime / sf::seconds(1.f) > 2.f)
            {
                box1.round(10.f, 2, 0);
                box2.round(10.f, 2, 0);
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
                musiclevel3.setVolume(20.f);
            }
            else
            {
                musiclevel3.setVolume(0.f);
            }
            if (stage == 0 && levelTime / sf::seconds(1.f) > 3.f)
            {
                static sf::Time loopTime = sf::seconds(0.f);
                if (gameTime - loopTime > sf::seconds(0.4f))
                {
                    boxleft.serpentine(10.f, 1, 0);
                    boxright.serpentine(10.f, 1, pi);
                    loopTime = gameTime;
                }
                if (levelTime / sf::seconds(1.f) > 5.f)
                {
                    stage = 1;
                    levelTime = sf::seconds(0.f);
                }
            }
            if (stage == 1 && levelTime / sf::seconds(1.f) > 4.f)
            {
                boxup.serpentine(10.f, 1, pi / 2);
                boxdown.serpentine(10.f, 1, -pi / 2);
                boxleft.sector(10.f, 3, 0);
                boxright.sector(10.f, 3, pi);
                stage = 2;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 2 && levelTime / sf::seconds(1.f) > 4.f)
            {
                items.push_back(std::unique_ptr<Item>(new Item({ 960.f,540.f }, 5, 6.f)));
                boxup.plane({ 30.f,100.f }, 1, pi / 2, 0.7f);
                boxleft.plane({ 100.f,30.f }, 1, 0, 0.7f);
                boxdown.follow(40.f, 3, &player);
                stage = 3;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 3 && levelTime / sf::seconds(1.f) > 6.f)
            {
                boxright.boom(40.f, 3, pi, 1.f, 3);
                boxleft.boom(40.f, 3, 0, 1.f, 3);
                stage = 4;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 4 && levelTime / sf::seconds(1.f) > 3.f)
            {
                items.push_back(std::unique_ptr<Item>(new Item({ 960.f,540.f }, 5, 6.f)));
                stage = 5;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 5)
            {
                static sf::Time loopTime = sf::seconds(0.f);
                if (gameTime - loopTime > sf::seconds(0.5f))
                {
                    box1.round(10.f, 2, 0);
                    box2.round(10.f, 2, 0);
                    loopTime = gameTime;
                }
                if (levelTime / sf::seconds(1.f) > 6.f)
                {
                    stage = 6;
                    levelTime = sf::seconds(0.f);
                }
            }
            if (stage == 6 && levelTime / sf::seconds(1.f) > 2.f)
            {
                islottery = true;
                iscoin = true;
                stage = 7;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 7 && !islottery)
            {
                items.push_back(std::unique_ptr<Item>(new Item({ 860.f,540.f }, 0, 6.f)));
                items.push_back(std::unique_ptr<Item>(new Item({ 960.f,540.f }, 0, 6.f)));
                items.push_back(std::unique_ptr<Item>(new Item({ 1060.f,540.f }, 0, 6.f)));
                isboss = true;
                stage = 8;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 8 && levelTime / sf::seconds(1.f) > 3.f)
            {
                isboss = false;
                static sf::Time loopTime = sf::seconds(0.f);
                if (gameTime - loopTime > sf::seconds(0.5f))
                {
                    boxboss.sector(10.f, 2, 0);
                    boxboss.sector(10.f, 2, pi);
                    boxboss.sector(10.f, 2, pi / 2);
                    boxboss.sector(10.f, 2, -pi / 2);
                    loopTime = gameTime;
                }
                if (levelTime / sf::seconds(1.f) > 8.f)
                {
                    stage = 9;
                    levelTime = sf::seconds(0.f);
                }
            }
            if (stage == 9 && levelTime / sf::seconds(1.f) > 3.f)
            {
                static sf::Time loopTime = sf::seconds(0.f);
                if (gameTime - loopTime > sf::seconds(0.08f))
                {
                    boxboss.shoot(10.f, 1, (levelTime / sf::seconds(1.f) - 3.f) / 5.f * 2 * pi);
                    boxboss.shoot(10.f, 1, (levelTime / sf::seconds(1.f) - 3.f) / 5.f * 2 * pi + 2 * pi / 3);
                    boxboss.shoot(10.f, 1, (levelTime / sf::seconds(1.f) - 3.f) / 5.f * 2 * pi - 2 * pi / 3);
                    loopTime = gameTime;
                }
                if (levelTime / sf::seconds(1.f) > 15.f)
                {
                    stage = 10;
                    levelTime = sf::seconds(0.f);
                }
            }
            if (stage == 10 && levelTime / sf::seconds(1.f) > 3.f)
            {
                boxboss.boom(40.f, 3, 0, 0.7f, 3);
                boxboss.boom(40.f, 3, pi, 0.7f, 3);
                boxboss.boom(40.f, 3, pi / 2, 0.7f, 3);
                boxboss.boom(40.f, 3, -pi / 2, 0.7f, 3);
                boxboss.boom(40.f, 3, 0, 0.7f, 3);
                boxboss.boom(40.f, 3, pi, 0.7f, 3);
                boxboss.boom(40.f, 3, pi / 2, 0.7f, 3);
                boxboss.boom(40.f, 3, -pi / 2, 0.7f, 3);
                stage = 11;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 11 && levelTime / sf::seconds(1.f) > 5.f)
            {
                boxboss.plane({ 100.f,30.f }, 1, 0, 0.6f);
                boxboss.plane({ 100.f,30.f }, 1, pi, 0.6f);
                boxboss.plane({ 30.f,100.f }, 1, pi / 2, 0.6f);
                boxboss.plane({ 30.f,100.f }, 1, -pi / 2, 0.6f);
                stage = 12;
                levelTime = sf::seconds(0.f);
            }
            if (stage == 12 && levelTime / sf::seconds(1.f) > 5.f)
            {
                static sf::Time loopTime = sf::seconds(0.f);
                if (gameTime - loopTime > sf::seconds(0.08f))
                {
                    boxboss.shoot(10.f, 1, pi * sin((levelTime / sf::seconds(1.f) - 5.f) / 5.f * 1 * pi));
                    boxboss.shoot(10.f, 1, pi * sin((levelTime / sf::seconds(1.f) - 5.f) / 5.f * 1 * pi) + 2 * pi / 3);
                    boxboss.shoot(10.f, 1, pi * sin((levelTime / sf::seconds(1.f) - 5.f) / 5.f * 1 * pi) - 2 * pi / 3);
                    loopTime = gameTime;
                }
                if (levelTime / sf::seconds(1.f) > 15.f)
                {
                    stage = 13;
                    levelTime = sf::seconds(0.f);
                }
            }
            if (stage == 13 && levelTime / sf::seconds(1.f) > 4.f)
            {
                static sf::Time loopTime = sf::seconds(0.f);
                static float phase = std::uniform_real_distribution(-pi / 3, pi / 3)(gen);
                if (gameTime - loopTime > sf::seconds(0.1f))
                {
                    boxboss.serpentine(10.f, 1, phase + (levelTime / sf::seconds(1.f) - 5.f) / 5.f * pi);
                    boxboss.serpentine(10.f, 1, phase + (levelTime / sf::seconds(1.f) - 5.f) / 5.f * pi + 2 * pi / 3);
                    boxboss.serpentine(10.f, 1, phase + (levelTime / sf::seconds(1.f) - 5.f) / 5.f * pi - 2 * pi / 3);
                    loopTime = gameTime;
                }
                if (levelTime / sf::seconds(1.f) > 15.f)
                {
                    stage = 14;
                    levelTime = sf::seconds(0.f);
                }
            }
            if (stage == 14 && levelTime / sf::seconds(1.f) > 1.f)
            {
                static sf::Time loopTime = sf::seconds(0.f);
                boxboss.shoot(10.f, 1, (levelTime / sf::seconds(1.f) - 1.f) / 5.f * 10 * pi);
                loopTime = gameTime;
                if (levelTime / sf::seconds(1.f) > 15.f)
                {
                    stage = 15;
                    levelTime = sf::seconds(0.f);
                }
            }
            if (stage == 15 && levelTime / sf::seconds(1.f) > 3.f)
            {
                static sf::Time loopTime = sf::seconds(0.f);
                if (gameTime - loopTime > sf::seconds(0.2f))
                {
                    boxboss.follow(40.f, 3, &player);
                    loopTime = gameTime;
                }
                if (levelTime / sf::seconds(1.f) > 8.f)
                {
                    stage = 16;
                    levelTime = sf::seconds(0.f);
                }
            }
            if (stage == 16 && levelTime / sf::seconds(1.f) > 1.f)
            {
                static sf::Time loopTime = sf::seconds(0.f);
                if (gameTime - loopTime > sf::seconds(0.2f))
                {
                    bullets.push_back(std::unique_ptr<RectangleBullet>(new RectangleBullet({ 50.f,50.f }, { 960.f,400.f }, sf::Color::Red, 30.f, (levelTime / sf::seconds(1.f) - 3.f) / 5.f * 2 * pi, 3, 0.5f)));
                    bullets.push_back(std::unique_ptr<RectangleBullet>(new RectangleBullet({ 50.f,50.f }, { 960.f,400.f }, sf::Color::Red, 30.f, (levelTime / sf::seconds(1.f) - 3.f) / 5.f * 2 * pi + 2 * pi / 3, 3, 0.5f)));
                    bullets.push_back(std::unique_ptr<RectangleBullet>(new RectangleBullet({ 50.f,50.f }, { 960.f,400.f }, sf::Color::Red, 30.f, (levelTime / sf::seconds(1.f) - 3.f) / 5.f * 2 * pi - 2 * pi / 3, 3, 0.5f)));
                    loopTime = gameTime;
                }
                if (levelTime / sf::seconds(1.f) > 8.f)
                {
                    stage = 17;
                    levelTime = sf::seconds(0.f);
                }
            }
            if (stage == 17 && levelTime / sf::seconds(1.f) > 1.f)
            {
                static sf::Time loopTime = sf::seconds(0.f);
                if (gameTime - loopTime > sf::seconds(0.2f))
                {
                    boxboss.round(10.f, 2, -50.f);
                    boxboss.round(10.f, 2, -50.f);
                    loopTime = gameTime;
                }
                if (levelTime / sf::seconds(1.f) > 6.f)
                {
                    stage = 18;
                    levelTime = sf::seconds(0.f);
                }
            }
            if (stage == 18 && levelTime / sf::seconds(1.f) > 5.f)
            {
                isover = true;
            }
        }
        if (level == 4)
        {
            if (isover)
            {
                musiclevel4.stop();
                continue;
            }
            if (musiclevel4.getStatus() != sf::Music::Status::Playing && !isPaused && !islottery && !isclock)
            {
                musiclevel4.setLooping(true);
                musiclevel4.play();
            }
            if (isPaused || islottery || isclock)
            {
                musiclevel4.pause();
            }
            if (ismusic)
            {
                musiclevel4.setVolume(20.f);
            }
            else
            {
                musiclevel4.setVolume(0.f);
            }
            if (!isgap && levelTime / sf::seconds(1.f) > 4 * (float)exp(-0.5f * degree) + 2.f)
            {
                if (stage == 8)
                {
                    if (bullets.size() == 0 || levelTime / sf::seconds(1.f) > 8.f)
                    {
                        isgap = true;
                        degree++;
                        stage = 0;
                        levelTime = sf::seconds(0.f);
                        items.push_back(std::unique_ptr<Item>(new Item({ 960.f,540.f }, 0, 6.f)));
                    }
                    continue;
                }
                if (stage == 3 || stage == 6)
                {
                    items.push_back(std::unique_ptr<Item>(new Item({ 960.f,540.f }, 5, 6.f)));
                }
                switch (std::uniform_int_distribution<int>(1, 10)(gen))
                {
                case 1:
                    box1.round(10.f, 2, 0);
                    box2.round(10.f, 2, 0);
                    boxleft.sector(10.f, 3, 0);
                    boxright.sector(10.f, 3, pi);
                    break;
                case 2:
                    boxleft.boom(40.f, 2, 0, 2.f, degree > 3 ? 5 : 3);
                    boxright.boom(40.f, 2, pi, 2.f, degree > 3 ? 5 : 3);
                    boxup.serpentine(10.f, 1, pi / 2);
                    boxdown.serpentine(10.f, 1, -pi / 2);
                    break;
                case 3:
                    boxleft.plane({ 100.f,30.f }, degree > 3 ? 1 : 2, 0, 0.4f * (float)exp(-degree) + 0.2f);
                    boxright.plane({ 100.f,30.f }, degree > 3 ? 1 : 2, pi, 0.4f * (float)exp(-degree) + 0.2f);
                    boxup.follow(40.f, 2, &player);
                    boxup.follow(40.f, 3, &player);
                    boxdown.follow(40.f, 2, &player);
                    boxdown.follow(40.f, 3, &player);
                    break;
                case 4:
                    box1.boom(40.f, 1, pi / 2, 2.f, degree > 3 ? 5 : 3);
                    box2.boom(40.f, 1, pi / 2, 2.f, degree > 3 ? 5 : 3);
                    break;
                case 5:
                    boxup.randomfall({300.f,150.f}, 2, pi / 2);
                    boxdown.randomfall({ 300.f,150.f }, 2, -pi / 2);
                    boxright.follow(40.f, degree > 3 ? 3 : 2, &player);
                    boxleft.follow(40.f, degree > 3 ? 3 : 2, &player);
                    break;
                case 6:
                    boxleft.randomfall({ 200.f,140.f }, 2, 0);
                    boxright.randomfall({ 200.f,140.f }, 2, pi);
                    boxup.follow(40.f, degree > 3 ? 3 : 2, &player);
                    boxdown.follow(40.f, degree > 3 ? 3 : 2, &player);
                    break;
                case 7:
                    boxboss.round(10.f, 2, 10.f);
                    boxboss.round(10.f, 3, -50.f);
                    boxup.sector(10.f, degree > 3 ? 3 : 2, pi / 2);
                    boxdown.sector(10.f, degree > 3 ? 3 : 2, -pi / 2);
                    break;
                case 8:
                    boxup.plane({ 30.f,100.f }, degree > 3 ? 1 : 2, pi / 2, 0.4f * (float)exp(-degree) + 0.2f);
                    boxdown.plane({ 30.f,100.f }, degree > 3 ? 1 : 2, -pi / 2, 0.4f * (float)exp(-degree) + 0.2f);
                    box1.follow(40.f, 2, &player);
                    box2.follow(40.f, 2, &player);
                    break;
                case 9:
                    boxup.sector(10.f, 2, pi / 2);
                    boxup.sector(10.f, 3, pi / 2);
                    boxdown.sector(10.f, 2, -pi / 2);
                    boxdown.sector(10.f, 3, -pi / 2);
                    boxleft.sector(10.f, 2, 0);
                    boxright.sector(10.f, 2, pi);
                    break;
                case 10:
                    boxleft.plane({ 100.f,30.f }, 1, 0, 0.6f * (float)exp(-degree) + 0.2f);
                    boxright.plane({ 100.f,30.f }, 1, pi, 0.6f * (float)exp(-degree) + 0.2f);
                    boxup.plane({ 30.f,100.f }, 1, pi / 2, 0.6f * (float)exp(-degree) + 0.2f);
                    boxdown.plane({ 30.f,100.f }, 1, -pi / 2, 0.6f * (float)exp(-degree) + 0.2f);
                    break;
                }
                stage++;
                levelTime = sf::seconds(0.f);
            }
        }
    }
    //退出
    musicmain.stop();
    musiclevel1.stop();
    musiclevel2.stop();
    musiclevel3.stop();
    musiclevel4.stop();
    std::cout << "按Enter键退出";
    std::cin.get();
    return 0;
}
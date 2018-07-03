#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include <RealEngine/network.h>

#include <chrono>

class Player
{
public:
    Player( int id, re::Color n_color, re::Point2f n_pos = {-1,-1} )
    {
        this->color = n_color;
        if( n_pos != re::Point2f(-1,-1) )
        {
            pos.push_back(n_pos);
            n_pos.x -= 1;
            pos.push_back(n_pos);
        }
        else
        {
            pos.push_back( re::Point2f( rand() % 5, rand() % 5 ) );
            pos.push_back( re::Point2f( pos[0].x + 1, pos[0].y ) );
        }

        input = re::Point2f(1,0);
        alive = true;
    }

    void update()
    {
        if( !alive )
        {
            return;
        }
        input = new_input;

        for( int i = pos.size()-1; i > 0; i-- )
        {
            pos[i].x = int(pos[i-1].x) % 20;
            pos[i].y = int(pos[i-1].y) % 20;
            if(pos[i].x < 0)
            {
                pos[i].x += 20;
            }
            if(pos[i].y < 0)
            {
                pos[i].y += 20;
            }
        }

        pos[0].x = int(pos[0].x + input.x) % 20;
        pos[0].y = int(pos[0].y + input.y) % 20;
        if(pos[0].x < 0)
        {
            pos[0].x += 20;
        }
        if(pos[0].y < 0)
        {
            pos[0].y += 20;
        }
    }

    void draw()
    {
        if( !alive )
        {
            return;
        }
        for( int i = 0; i < pos.size(); i++ )
        {
            re::Color col = color;
            float coff = float(pos.size() - i + pos.size()) / float(pos.size()*2);
            col.r *= coff;
            col.g *= coff;
            col.b *= coff;
            re::draw_rectangle(
                pos[i] * 20,
                re::Point2f(20,20),
                col
            );
        }
    }

    int is_collide( std::vector<re::Point2f> c_poses )
    {
        for( int j = 0; j < c_poses.size(); j++ )
        {
            for( int i = 1; i < pos.size(); i++ )
            {
                if( pos[i] == c_poses[j] )
                {
                    return i;
                }
            }
        }
        return -1;
    }

    re::Color color;
    re::Point2f input = {0,1};
    re::Point2f new_input = {0,1};
    std::vector<re::Point2f> pos;
    bool alive;
};

enum GameState{
    menu,
    client,
    server
};

re::Color get_player_color( int id )
{
    switch(id)
    {
    case 0: return re::YELLOW;
    case 1: return re::BLUE;
    case 2: return re::GRAY;
    case 3: return re::DARKGREEN;
    default:return re::BLACK;
    }
}

class MainApp : public re::IBaseApp{
public:
    std::vector<Player> players;
    std::vector<re::Point2f> walls;
    const size_t max_players = 2;
    std::chrono::time_point<std::chrono::steady_clock> snake_update_time;
    GameState gameState;

    std::string addrres;
    re::TCPClientPtr tcp_client;
    re::TCPServerPtr tcp_server;

    re::Point2f eat_pos;

    void on_recive_client( std::vector<char> msg )
    {
        size_t wall_count;
        int player_index;
        switch( msg[0] )
        {
        case 0x08:
            eat_pos.x = msg[1];
            eat_pos.y = msg[2];
            break;
        case 0x07:
            player_index = msg[1];
            players[player_index].alive = msg[2];
            break;
        case 0x06:
            players[msg[1]].alive = msg[2];
            break;
        case 0x05:
            wall_count = msg[1];
            for( int i = 0; i < wall_count; i++ )
            {
                walls.push_back( re::Point2f( msg[2 + (i * 2)], msg[3 + (i * 2)] ) );
            }
            break;
        case 0x04:
            players.push_back( Player(
                players.size(),
                get_player_color(players.size()),
                re::Point2f(msg[1],msg[2]) ) );
            break;
        case 0x02:
            for( int i = 0; i <= msg[1]; i++ )
            {
                players.push_back( Player(
                    i,
                    get_player_color(i) ) );
            }
            break;
        case 0x01:
            size_t index = msg[1];
            size_t len = msg[2];
            if( players[index].pos.size() < len )
            {
                for( int i = 0; i < players[index].pos.size() - len; i++ )
                {
                    players[index].pos.push_back( re::Point2f(0,0) );
                }
            }
            else if( players[index].pos.size() > len )
            {
                while(players[index].pos.size() != len)
                {
                    players[index].pos.pop_back();
                }
            }
            for( int i = 0; i < len; i++ )
            {
                players[index].pos[i].x = msg[3 + (i*2)];
                players[index].pos[i].y = msg[4 + (i*2)];
            }
            break;
        }
    }

    void sync_dead( int id, int alive )
    {
        std::vector<char> msg;
        msg.push_back(0x07);
        msg.push_back(id);
        msg.push_back(alive);
        for( int i = 1; i < players.size(); i++ )
        {
            tcp_server->send( i-1, msg );
        }
    }

    bool is_collide_with_snake( re::Point2f pos )
    {
        for( auto player : players )
        {
            for( auto block : player.pos )
            {
                if( pos == block )
                {
                    return true;
                }
            }
        }
        return false;
    }

    void gen_eat_pos()
    {
        re::Point2f pos = re::Point2f(2,2);//(rand()%20,rand()%20);
        size_t offset = 0;
        while(is_collide_with_snake(pos))
        {
            if( offset >= 20 )
            {
                pos.y++;
                offset = 0;
            }
            else
            {
                pos.x++;
                offset++;
            }
            pos.x = int(pos.x) % 20;
            pos.y = int(pos.y) % 20;
        }
        eat_pos = pos;
        sync_eat();
    }

    void sync_eat()
    {
        std::vector<char> msg;
        msg.push_back(0x08);
        msg.push_back(eat_pos.x);
        msg.push_back(eat_pos.y);
        for( int i = 1; i < players.size(); i++ )
        {
            tcp_server->send( i-1, msg );
        }
    }

    void sync_pos()
    {
        for( int i = 1; i < players.size(); i++ )
        {
            for( int j = 0; j < players.size(); j++ )
            {
                std::vector<char> msg;
                msg.push_back(0x01);
                msg.push_back(j);
                msg.push_back(players[j].pos.size());
                for( auto pos : players[j].pos )
                {
                    msg.push_back( pos.x );
                    msg.push_back( pos.y );
                }

                tcp_server->send( i-1, msg );
            }
        }
    }

    void on_connection( int id )
    {
        std::vector<char> msg;
        msg.push_back(0x02); // you index
        msg.push_back(players.size()); // you index
        tcp_server->send(id, msg);

        int x = rand()%10;
        int y = rand()%10;

        players.push_back( Player( 
            id+1,
            get_player_color(id+1),
            re::Point2f(x,y) ) );

        sync_pos();
        sync_eat();
    }

    void on_recive_server( int id, std::vector<char> msg )
    {
        switch( msg[0] )
        {
        case 0x03:
            if( ((msg[1] * -1) == players[id+1].input.x) ||
                ((msg[2] * -1) == players[id+1].input.y) )
            {
                break;
            }
            players[id+1].new_input.x = msg[1];
            players[id+1].new_input.y = msg[2];
            break;
        }
    }

    void setup() override {
        gameState = GameState::menu;
        snake_update_time = std::chrono::steady_clock::now();
        re::set_fps(60);
        re::background(re::WHITE);
    }

    void update() override {
        if( gameState == GameState::server )
        {
            int time_milils = (std::chrono::duration_cast<std::chrono::microseconds>
                (std::chrono::steady_clock::now() - snake_update_time)).count();

            if( time_milils > 500'000 )
            {
                std::vector<re::Point2f> heads;
                std::vector<re::Point2f> n_walls;
                for( int i = 0; i < players.size(); i++ )
                {
                    if( !players[i].alive )
                    {
                        continue;
                    }
                    players[i].update();
                    if( players[i].pos[0] == eat_pos )
                    {
                        players[i].pos.push_back( players[i].pos[players[i].pos.size()-1] );
                        gen_eat_pos();
                    }
                    heads.push_back( players[i].pos[0] );
                }

                for( int i = 0; i < players.size(); i++ )
                {
                    for( auto wall : walls )
                    {
                        if( players[i].pos[0] == wall )
                        {
                            players[i].alive = false;
                            sync_dead(i,0);
                        }
                    }
                    if( !players[i].alive )
                    {
                        continue;
                    }
                    int collide_pos = players[i].is_collide( heads );
                    if( collide_pos != -1 )
                    {
                        size_t old_size = players[i].pos.size();
                        for( int j = collide_pos; j < old_size; j++ )
                        {
                            re::Point2f wall = players[i].pos[players[i].pos.size()-1];
                            players[i].pos.pop_back();
                            n_walls.push_back(wall);
                        }
                    }

                    if(players[i].pos.size() == 0)
                    {
                        players[i].alive = false;
                    }
                }

                if( n_walls.size() > 0 )
                {
                    std::vector<char> msg_wall;
                    msg_wall.push_back(0x05);
                    msg_wall.push_back(n_walls.size());
                    for( auto wall : n_walls )
                    {
                        msg_wall.push_back( wall.x );
                        msg_wall.push_back( wall.y );
                        walls.push_back( wall );
                    }

                    for( int i = 1; i < players.size(); i++ )
                    {
                        tcp_server->send( i-1, msg_wall );
                    }
                }
                sync_pos();
                snake_update_time = std::chrono::steady_clock::now();
            }
        }
    }

    void display() override {
        if( gameState == GameState::menu ) {
            re::draw_text(
                re::Point2f(400,40),
                "press 'S' to start server",
                re::BLACK
            );

            re::draw_text(
                re::Point2f(400,20),
                "press 'C' to connect",
                re::BLACK
            );

            re::draw_text(
                re::Point2f(400, 60),
                addrres,
                re::BLACK
            );
        }
        for( auto wall : walls )
        {
            re::draw_rectangle(
                wall * 20,
                re::Point2f(20,20),
                re::BLACK
            );
        }
        for( int i = 0; i < players.size(); i++ )
        {
            players[i].draw();
        }
        re::draw_rectangle(
            eat_pos * 20,
            re::Point2f(20,20),
            re::GREEN );

        switch( gameState )
        {
        case GameState::client:
            break;
        case GameState::server:
            break;
        }

        for( int i = 1; i <= 20; i++ )
        {
            re::draw_line( 20 * i, 0, 20 * i, 400, re::BLACK );
            re::draw_line( 0, 20 * i, 400, 20 * i, re::BLACK );
        }
    }

    void on_key_pressed(re::Key key) override
    {
        if((key==re::Key::W ||
            key==re::Key::S ||
            key==re::Key::A ||
            key==re::Key::D ) &&
            gameState != GameState::menu)
        {
            if(gameState == GameState::client)
            {
                std::vector<char> msg;
                msg.push_back(0x03);
                msg.push_back( (key == re::Key::D ? 1 : 0) - 
                               (key == re::Key::A ? 1 : 0) );
                msg.push_back( (key == re::Key::S ? 1 : 0) - 
                               (key == re::Key::W ? 1 : 0) );
                tcp_client->send(msg);
            }
            else if( gameState == GameState::server )
            {
                re::Point2f new_input;
                new_input.x = ( (key == re::Key::D ? 1 : 0) - 
                                (key == re::Key::A ? 1 : 0) );
                new_input.y = ( (key == re::Key::S ? 1 : 0) - 
                                (key == re::Key::W ? 1 : 0) );
                if( ((new_input.x * -1) == players[0].input.x) ||
                    ((new_input.y * -1) == players[0].input.y) )
                {
                    return;
                }
                players[0].new_input = new_input;
            }
            return;
        }
        if( gameState == GameState::menu &&
          ((key >= re::Key::Num0 && key <= re::Key::Num9) ||
            key == re::Key::P || 
            key == re::Key::O ))
        {
            if( key == re::Key::P)
            {
                addrres += '.';
                return;
            }
            if( key == re::Key::O)
            {
                addrres = addrres.substr( 0, addrres.size() - 1 );//addrres.substr( 0, addrres.size()-4 );
                return;
            }
            addrres += ( '0' + (int)key - (int)re::Key::Num0 );
            return;
        }
        switch( gameState )
        {
        case GameState::menu:
            switch( key )
            {
            case re::Key::S:
                tcp_server = re::TCPServer::get();
                tcp_server->set_connect_callback( std::bind( 
                    &MainApp::on_connection, 
                    this, 
                    std::placeholders::_1 ) );

                tcp_server->set_recive_callback( std::bind(
                    &MainApp::on_recive_server,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2 ) );

                tcp_server->setup(11999);

                gameState = GameState::server;

                players.push_back( Player(0,re::YELLOW, re::Point2f(2,2) ) );
                break;
            case re::Key::C:
                tcp_client = re::TCPClient::get();
                tcp_client->set_recive_callback( std::bind(
                    &MainApp::on_recive_client,
                    this,
                    std::placeholders::_1 ) );

                tcp_client->connect( addrres, 11999 );

                gameState = GameState::client;
                break;
            }
            break;
        }
    }

    void on_mouse_move(int,int) override
    {

    }
};
 
int main(int argc, char const *argv[])
{
    re::setWindowName( "Snake" );
    re::runApp( 600, 400, std::make_shared<MainApp>() );
    return 0;
}

// Básicos
#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <random>
#include <cmath>
#include <queue>
#include <functional>

// Sockets
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Threads
#include <thread>
#include <chrono>

#define PORT 8090

class TaskQueue
{
    public:
        void enqueue( std::function<void()> task ) {
            queue.emplace(std::move(task));
        }
        void processNext() {
            if (!queue.empty()) {
                auto task = std::move(queue.front());
                queue.pop();
                task(); // Execute the task
            }
        }
        bool empty() {
            return queue.empty();
        }

    private:
        std::queue<std::function<void()>> queue;
};

void handle_client( int client_socket, int workload )
{
    char buffer[30000] = {0};
    read(client_socket, buffer, 30000);

    // Cria uma resposta HTTP básica
    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<html>\n"
        "    <head>\n"
        "        <title> Programação concorrente 2024.2 </title>\n"
        "    </head>\n"
        "    <body>\n"
        "        <h1> Programação concorrente 2024.2 </h1>\n"
        "        <p> <b>Código:</b> ICP361 </p>\n"
        "        <p> <b>Professora:</b> Silvana Rosseto </p>\n"
        "        <p> Universidade Federal do Rio de Janeiro - <b>UFRJ</b> </p>\n"
        "        <p> <b>Carga Horária Proposta / Créditos:</b> 45h teóricas, 15h práticas (4 créditos) </p>\n"
        "        <h2>Ementa</h2>\n"
        "        <ul>\n"
        "            <li>Técnicas de programação concorrente: multiprocessos, multithreading.</li>\n"
        "            <li>Comunicação via variáveis compartilhadas e troca de mensagens.</li>\n"
        "            <li>Sincronização por exclusão mútua e condicional e métodos de trava: locks, semáforos, variáveis de condição.</li>\n"
        "            <li>Problemas clássicos da concorrência.</li>\n"
        "            <li>Deadlock, starvation, thread safety.</li>\n"
        "            <li>Programação assíncrona: futuros, promessas, async/await.</li>\n"
        "            <li>Modelagem, teste e avaliação de programas concorrentes.</li>\n"
        "            <li>Ambientes de programação concorrente.</li>\n"
        "        </ul>\n"
        "    </body>\n"
        "</html>\r\n\r\n";

    auto now = std::chrono::system_clock::now();

    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::cout << "[ "
              << std::put_time(std::localtime(&currentTime), "%Y-%m-%d %H:%M:%S")
              << " ] Requisição recebida\n";

    std::cout << "Carga de trabalho: " << workload << " ms\n" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds( workload ));

    // Envia a resposta ao cliente
    send(client_socket, response.c_str(), response.length(), 0);

    // Fecha a conexão com o cliente
    close(client_socket);
}

int main( int argc, char *argv[] )
{
    std::cout << R"( _____                 _     _              _   _ _____ ___________ )" << std::endl;
    std::cout << R"(/  ___|               (_)   | |            | | | |_   _|_   _| ___ \)" << std::endl;
    std::cout << R"(\ `--.  ___ _ ____   ___  __| | ___  _ __  | |_| | | |   | | | |_/ /)" << std::endl;
    std::cout << R"( `--. \/ _ \ '__\ \ / / |/ _` |/ _ \| '__| |  _  | | |   | | |  __/ )" << std::endl;
    std::cout << R"(/\__/ /  __/ |   \ V /| | (_| | (_) | |    | | | | | |   | | | |    )" << std::endl;
    std::cout << R"(\____/ \___|_|    \_/ |_|\__,_|\___/|_|    \_| |_/ \_/   \_/ \_|    )" << std::endl  << std::endl;

    std::cout << "Programação concorrente 2024.2 - UFRJ\n\n";

    if (argc < 2) {
        printf("Uso: %s <workload-duration-in-ms>\n", argv[0]);
        return 1;
    }

    int mean = atof(argv[1]);

    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Cria o socket do servidor
    if ( ( server_fd = socket( AF_INET, SOCK_STREAM, 0 ) ) == 0 )
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Define as propriedades do endereço do servidor
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Associa o socket à porta 8090
    if ( bind( server_fd, (struct sockaddr*) &address, sizeof(address) ) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Inicia o servidor em modo de escuta
    if ( listen( server_fd, 10 ) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Servidor escutando em http://localhost:" << PORT << "\n" << std::endl;

    // Desvio padrão da Gaussiana.
    double stddev = 0.2*mean;

    std::random_device rd;  // Semente
    std::mt19937 gen(rd()); // Gerador de números aleatórios

    // Distribuição normal para estimar o tempo de trabalho.
    std::normal_distribution<> dist(mean, stddev);

    TaskQueue q;

    // Loop para aceitar e processar conexões
    while (true)
    {
        if (!q.empty()) {
            // std::cout << "Fila nao esta vazia\n";
            q.processNext();
        }

        // Chamada bloqueante: Fluxo trava aqui até que chegue uma mensagem através do socket.
        client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if ( client_socket < 0)
        {
            perror("Accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        // Carga de trabalho da requisição.
        int workload = std::round(dist(gen));

        // Processa a requisição HTTP do cliente
        // QUando chega requisição, em vez de executar a handle_client diretamente
        handle_client(client_socket, workload);
        q.enqueue( [client_socket, workload]() {
            handle_client(client_socket, workload);
        } );
    }

    return 0;
}

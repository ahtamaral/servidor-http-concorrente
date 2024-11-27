
// Básicos
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <queue>
#include <functional>
#include <iomanip>

// Sockets
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Threads
#include <mutex>
#include <thread>
#include <chrono>
#include <condition_variable>

#include <random>

#define PORT 8090

// Thread pool class
class ThreadPool {

public:
    ThreadPool(size_t thread_count);
    ~ThreadPool();

    void enqueue(std::function<void()> task);

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
    int id = 1;
};

// Costrutor da pool.
ThreadPool::ThreadPool(size_t thread_count) : stop(false) {

    for (size_t i = 0; i < thread_count; ++i) {
        workers.emplace_back([this]() {

            queue_mutex.lock();
            int myId = id;
            std::cout << "Inicializando thread #" << myId << std::endl;
            id++;
            queue_mutex.unlock();

            while (true) {
                std::function<void()> task;

                {
                    queue_mutex.lock();
                    std::cout << "#" << myId << " assumiu a tarefa.\n";
                    queue_mutex.unlock();

                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                     
                    this->condition.wait(lock, [this]() { return this->stop || !this->tasks.empty(); });

                    if (this->stop && this->tasks.empty()) return;

                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }

                task();
            }
        });
    }
}

// Destructor
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }

    condition.notify_all();

    for (std::thread &worker : workers) {
        worker.join();
    }
}

// Add task to queue
void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.emplace(std::move(task));
    }

    condition.notify_one();
}

// Processa a requisição HTTP do cliente
void handle_client(int client_socket, int workload) {
    
    char buffer[30000] = {0};
    read(client_socket, buffer, 30000);

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

    send(client_socket, response.c_str(), response.length(), 0);

    close(client_socket);
}


int main( int argc, char *argv[] )
{
    if (argc < 3) {
        printf("Uso: %s <workload-duration-in-ms> <n-threads>\n", argv[0]);
        return 1;
    }

    std::cout << R"( _____                 _     _              _   _ _____ ___________ )" << std::endl;
    std::cout << R"(/  ___|               (_)   | |            | | | |_   _|_   _| ___ \)" << std::endl;
    std::cout << R"(\ `--.  ___ _ ____   ___  __| | ___  _ __  | |_| | | |   | | | |_/ /)" << std::endl;
    std::cout << R"( `--. \/ _ \ '__\ \ / / |/ _` |/ _ \| '__| |  _  | | |   | | |  __/ )" << std::endl;
    std::cout << R"(/\__/ /  __/ |   \ V /| | (_| | (_) | |    | | | | | |   | | | |    )" << std::endl;
    std::cout << R"(\____/ \___|_|    \_/ |_|\__,_|\___/|_|    \_| |_/ \_/   \_/ \_|    )" << std::endl  << std::endl;

    std::cout << "Programação concorrente 2024.2 - UFRJ\n\n";
    
    int mean = atoi(argv[1]);
    int nthreads = atoi(argv[2]);

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
    std::cout << "Numero de threads do servidor: " << nthreads << "\n" << std::endl;

    ThreadPool pool( (size_t) nthreads );

    // Desvio padrão da Gaussiana.
    double stddev = 0.2*mean;

    std::random_device rd;  // Semente
    std::mt19937 gen(rd()); // Gerador de números aleatórios

    // Distribuição normal para estimar o tempo de trabalho.
    std::normal_distribution<> dist(mean, stddev);

    while (true) 
    {
        if ((client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) 
        {
            perror("Accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        // Carga de trabalho da requisição.
        int workload = std::round(dist(gen));

        pool.enqueue([client_socket, workload]() { handle_client(client_socket, workload); });
    }

    return 0;
}
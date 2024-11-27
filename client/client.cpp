#include <iostream>
#include <curl/curl.h>
#include <thread>
#include <vector>
#include <iomanip>  // For std::fixed and std::setprecision
#include <mutex>

// Indica se houve sucesso em TODAS as requisições feitas.
bool globalSuccess = true;
int sent = 0;
int successed = 0;
int failed = 0;

// Function to perform an HTTP request
void perform_request(const std::string &url) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        std::cout << "Realizando requisição para o servidor...\n";

        // Perform the request
        res = curl_easy_perform(curl);
        sent++;
        
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        auto curTimestamp = std::put_time(std::localtime(&currentTime), "[ %Y-%m-%d %H:%M:%S ] ");

        // Check for errors
        if(res != CURLE_OK) {
            std::cerr << curTimestamp << "curl_easy_perform() falhou. Status:" << curl_easy_strerror(res) << std::endl;
            globalSuccess = false;
            failed++;
        } else {
            std::cout << curTimestamp << "Resposta recebida com sucesso.\n" << std::endl;
            successed++;
        }

        // Clean up
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

int main( int argc, char *argv[] ) 
{

    if (argc < 2) {
        printf("Uso: %s <n-requests>\n", argv[0]);
        return 1;
    }

    unsigned int nReqs = (unsigned int) atoi(argv[1]);

    // Create a vector to hold threads
    std::vector<std::thread> threads;

    // URL a ser requisitada
    std::string url = "http://localhost:8090/";
    //std::string url = "http://172.17.0.4:8090/";

    auto start = std::chrono::high_resolution_clock::now();

    // Create threads for each URL
    for ( unsigned int i = 0; i < nReqs; i++) {
        threads.push_back(std::thread(perform_request, url));
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout <<  ( globalSuccess ? 
    "\nTodas as requisições foram concluídas com sucesso!\nDuração: " :
    "\nAlgumas requisições falharam." );

    float successRate = ( static_cast<float>( successed ) / static_cast<float>( sent ) )*100;
    std::cout << "Taxa de sucesso: " << successRate << " %\n";

    std::cout << duration.count()  << " nanosegundos.";

    std::cout << std::endl;

    return 0;
}

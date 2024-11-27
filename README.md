# Servidor HTTP simples

Implementa um servidor HTTP simples em alguns paradigmas diferentes: Sequencial, multithreaded e I/O multiplexado, assim como um cliente que realiza teste de carga nos servidores para obter seus tempos de resposta. Projeto final da disciplina programação concorrente.

Dependências são **pthread** e **libcurl**.

Rodando um servidor (Multithreaded, p. ex.):

```bash
cd multithreaded
make
./bin/server <n-threads>
```

Rodando o cliente:

```bash
cd client
make
./bin/client <n-requisições>
```
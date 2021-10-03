### Bathroom Access

**Makefile**
* "make" para compilar tanto o servidor (Q2) como o cliente (U2)
* "make clean" para remover os executáveis criados

**Programa**
* Para correr, devem ser abertos dois ou mais terminais, em que num se vai correr o servidor (./Q2) e noutro o cliente/s (./U2), de acordo com a usage apresentada.
* O nosso grupo conseguiu implementar todos os requisitos pedidos nesta segunda etapa, pelo que o programa pode ser invocado com qualquer combinação dos argumentos da linha de comandos.
* Os pedidos que foram recebidos pelo servidor, mas que não foram atendidos por este, por motivos de encerramento, serão considerados 2LATE, e o cliente receberá a mensagem CLOSD.
* Consideramos que fazia sentido que o programa de um cliente deixasse de enviar mais pedidos após receber o primeiro CLOSD, uma vez que estes já não seriam atendidos pois o Quarto de Banho teria já encerrado. No entanto, são recebidas todas as respostas (CLOSD) dos pedidos do cliente que já tiverem sido enviados. Assim, quando é recebido o primeiro CLOSD, o cliente apresenta a mensagem "Bathroom is not available (Server FIFO not found)", e deixa de enviar novos pedidos. Quando termina o tempo do cliente introduzido como parâmetro, é apresentada a mensagem "Client Time Ended...".
* O programa do Servidor termina após o tempo enviado como parâmetro, independentemente de ter ou não recebido qualquer pedido. Quando este é aberto, é apresentada a mensagem "Bathroom Opening...", e quando termina o tempo de funcionamento do Quarto de Banho, apresenta-se a mensagem "Bathroom Closing...".
* Cada cliente envia um novo pedido a cada 10 ms, e a duração do pedido resulta de uma escolha aleatório entre 10 e 50 ms.
* Os fifos criados para cada pedido são criados na pasta /tmp, e o fifo do servidor é criado no diretório introduzido como parâmetro.
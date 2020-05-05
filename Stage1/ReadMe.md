### Bathroom Access

**Makefile**
* "make" para compilar tanto o servidor (Q1) como o cliente (U1)
* "make clean" para remover os executáveis criados

**Programa**
* Para correr, devem ser abertos dois ou mais terminais, em que num se vai correr o servidor (./Q1) e noutro o cliente/s (./U1), de acordo com a usage apresentada.
* Nesta primeira etapa, o número de vagas do Quarto de Banho é ilimitado, e o nosso grupo conseguiu implementar todos os requisitos pedidos.
* Por esse motivo, quaisquer pedidos que sejam enviados ao servidor num momento em que este esteja aberto serão atendidos (IAMIN).
* Os pedidos que foram atendidos pelo servidor, mas que terminaram o seu uso do Quarto de Banho após a hora de encerramento deste, serão considerados 2LATE, e o cliente receberá a mensagem CLOSD.
* Consideramos que fazia sentido que o programa de um cliente deixasse de enviar mais pedidos após receber o primeiro CLOSD, uma vez que estes já não seriam atendidos pois o Quarto de Banho teria já encerrado. No entanto, são recebidas todas as respostas (CLOSD) dos pedidos do cliente que ainda estivessem em uso.
* O programa do Servidor termina após o tempo enviado como parâmetro, independentemente de ter ou não recebido qualquer pedido.
* Cada cliente envia um novo pedido a cada 250 ms (1/4 de segundo), e a duração do pedido resulta de uma escolha aleatório entre 10 e 50 ms. O intervalo entre pedidos pode ser alterado modificando o define REQUESTS_INTERVAL, de acordo com o parâmetro da função nanosleep().
* Os fifos criados para cada pedido são criados na pasta /tmp, e o fifo do servidor é criado no diretório introduzido como parâmetro.
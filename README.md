# ProjetoSantaBarbara
Levando em consideração o objetivo geral da pesquisa e os materiais dispostos para a pesquisa desenvolvi um projeto medidor de pH e cor que funcionam de forma integrada, 
usando um sensor TCS34725 RGB para determinar a cor de uma amostra líquida e um sensor entrada analógica para medir a tensão e calcular o pH. 
O código é escrito para uma placa Arduino, usando o Arduino Integrated Development Environment (IDE). 
A função setup() inicializa a comunicação serial e verifica se o sensor TCS34725 está conectado corretamente. 
A função loop() espera pela entrada do usuário através da porta serial, permitindo ao usuário escolher entre três opções: (1) medir a solução, (2) exibir os dados coletados ou (3) encerrar o programa. 
A função medicao() é chamada quando o usuário opta por medir novamente, ela realiza as medições e armazena os dados em uma std::list. 
A função exibirDados() é chamada quando o usuário opta por exibir os dados coletados, ela imprime os dados armazenados em std::list para o monitor serial. 
Existe um fator de calibração definido no início do código, que deve ser ajustado de acordo com o medidor de pH específico utilizado. 
A função medicao() lê a entrada analógica do pino A0, calcula a tensão e o pH e retorna uma string com os dados medidos. 
A medição da cor também é realizada na função medicao(), usando o sensor TCS34725. A medição da cor é feita verificando o valor do canal azul e vermelho até que ele caia abaixo de um certo limite, indicando que a cor desejada foi alcançada. 
Durante este processo, a medição de pH é realizada várias vezes, com cada resultado sendo adicionado à std::list. 
Após a conclusão da medição de cor,,os dados armazenados em std::list são impressos no monitor serial ao final do ciclo de medição. 
Com isso a montagem utiliza uma placa Arduino Mega2560 e os sensores trabalham de maneira complementar, fazendo com que o projeto se torne mais barato com soluções criadas na camada de software.

Реализация уровней компьютерных сетей, с дополнительным помехоустойчивым кодом Хемминга
1) L3 (Network Layer) - Оборачивание(распаковка) полученной информации для протокола IPv4 и передача пакета уровню L2(передача информационной части вызывающему уровню)
![image](https://github.com/user-attachments/assets/cd017a21-455a-47c1-afe1-584d83ab762f)
2) L2 (Data Link Layer) - Оборачивание(распаковка) полученной информации в пакеты PPP (Point-to-point Protocol) и передача пакета L1(передача информационной части вызывающему уровню)
![image](https://github.com/user-attachments/assets/88eda911-af5c-46b8-9e37-a90264c5d6bc)
3) L1 (Physical Layer) - Инициализация и подключение Serial Port, с дальнейшей побайтовой передачей информации и отправление пакета L1-получателя(передача информационной части вызывающему уровню)

Пример работы программы (для эмуляции кабельного подключения используется виртуальные последовательные порты VSPE): 
Отправитель - 
![image](https://github.com/user-attachments/assets/c73cb54f-66fb-4dc1-ab45-a9116a9c278d)
Получатель -
![image](https://github.com/user-attachments/assets/de847ea4-f066-48c8-8214-252eb2c0bb17)



P.S. - Вышеописанная система поддерживает передачу по полнодуплексному каналу (в две стороны одновременно), но для наглядности, принимающий и отправляющий клиенты были разделены

#include <gtest/gtest.h>
using namespace testing;

#include "Atm.hpp"
#include "BankMachine.hpp"
#include "InterfaceMachine.hpp"
#include "MessagesExample.hpp"

TEST(Messanger, main)
{
    bank_machine bank;
    interface_machine interface_hardware;

    atm machine(bank.get_sender(), interface_hardware.get_sender());

    std::thread bank_thread(&bank_machine::run, &bank);
    std::thread if_thread(&interface_machine::run, &interface_hardware);
    std::thread atm_thread(&atm::run, &machine);

    mess::Sender atmqueue(machine.get_sender());

    // bool quit_pressed = false;

    atmqueue.send(digit_pressed('1'));
    atmqueue.send(digit_pressed('9'));
    atmqueue.send(digit_pressed('3'));
    atmqueue.send(digit_pressed('7'));
    // while(!quit_pressed)
    // {
    //     char c = getchar();
    //     switch(c)
    //     {
    //         case '0':
    //         case '1':
    //         case '2':
    //         case '3':
    //         case '4':
    //         case '5':
    //         case '6':
    //         case '7':
    //         case '8':
    //         case '9':
    //             atmqueue.send(digit_pressed(c));
    //             break;
    //         case 'b':
    //             atmqueue.send(balance_pressed());
    //             break;
    //         case 'w':
    //             atmqueue.send(withdraw_pressed(50));
    //             break;
    //         case 'c':
    //             atmqueue.send(cancel_pressed());
    //             break;
    //         case 'q':
    //             quit_pressed = true;
    //             break;
    //         case 'i':
    //             atmqueue.send(card_inserted("acc1234"));
    //             break;
    //     }
    // }
    bank.done();
    machine.done();
    interface_hardware.done();
    atm_thread.join();
    bank_thread.join();
    if_thread.join();
}

#pragma once

#include <MessangerCore.hpp>

#include "MessagesExample.hpp"

class bank_machine
{
    mess::Receiver incoming;
    unsigned balance;

public:
    bank_machine()
        : balance(199)
    {
    }

    void done()
    {
        get_sender().send(mess::CloseQueue());
    }

    void run()
    {
        try
        {
            for(;;)
            {
                incoming.wait()
                    .handle<verify_pin>([&](verify_pin const& msg) {
                        if(msg.pin == "1937")
                        {
                            msg.atm_queue.send(pin_verified());
                        }

                        else
                        {
                            msg.atm_queue.send(pin_incorrect());
                        }
                    })
                    .handle<withdraw>([&](withdraw const& msg) {
                        // if(balance >= msg.amount)
                        // {
                        //     msg.atm_queue.send(withdraw_ok());
                        //     balance -= msg.amount;
                        // }
                        // else
                        // {
                        //     msg.atm_queue.send(withdraw_denied());
                        // }
                    })
                    .handle<get_balance>([&](get_balance const& msg) { msg.atm_queue.send(::balance(balance)); })
                    .handle<withdrawal_processed>([&](withdrawal_processed const& msg) {})
                    .handle<cancel_withdrawal>([&](cancel_withdrawal const& msg) {});
            }
        }
        catch(mess::CloseQueue const&)
        {
        }
    }

    mess::Sender get_sender()
    {
        return incoming;
    }
};
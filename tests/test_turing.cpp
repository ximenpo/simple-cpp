#include "simple/igloo.h"
using namespace igloo;

#include	<cmath>
#include	<cstring>
#include	"simple/turing.h"

bool            simple_turing_instruction_executor(turing_machine* machine, int instruction_address) {
    if(instruction_address > 5) {
        return  false;
    }
    if(instruction_address == 5) {
        machine->stop();
    }
    return  true;
}

Context(turing_context) {
    Spec(basic_status) {
        turing_machine  vm;
        AssertThat(vm.is_started(),	IsFalse());
        AssertThat(vm.is_paused(),	IsFalse());

        vm.start();
        AssertThat(vm.is_started(),	IsTrue());
        AssertThat(vm.is_paused(),	IsFalse());

        AssertThat(vm.run(true),    IsFalse());
    }

    Spec(basic_usage) {
        turing_machine  vm;
        vm.instruction_executor.bind(simple_turing_instruction_executor);

        vm.start();
        AssertThat(vm.run(false),   IsTrue());
        AssertThat(vm.is_started(), IsFalse());

        vm.start();
        AssertThat(vm.run(true),    IsTrue());
        AssertThat(vm.run(true),    IsTrue());
        AssertThat(vm.run(true),    IsTrue());
        vm.set_paused(true);
        AssertThat(vm.run(true),    IsFalse());
        vm.set_paused(false);
        AssertThat(vm.run(true),    IsTrue());
        AssertThat(vm.run(true),    IsTrue());
        AssertThat(vm.run(true),    IsTrue());
        AssertThat(vm.run(true),    IsFalse());
        AssertThat(vm.is_started(), IsFalse());
    }

};

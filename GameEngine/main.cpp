#include "GameClient/Doomsday.h"

int main() {
    glfwInit();
    Doomsday::GetInstance().Run();
    glfwTerminate();
    return 0;
}

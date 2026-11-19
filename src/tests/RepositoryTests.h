#ifndef REPOSITORYTESTS_H
#define REPOSITORYTESTS_H

class RepositoryTests {
public:
    static void runAll();

private:
    static void testDeviceRepository();
    static void testUserRepository();
    static void testLogRepository();
};

#endif // REPOSITORYTESTS_H

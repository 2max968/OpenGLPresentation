#include <iostream>
#include <chrono>
#include <conio.h>
#include <GLFW/glfw3.h>
#include "RenderFunctions.h"

int main()
{
    while (true)
    {
        // Auswahl des Unterprogramms
        char selectedProgram = 0;
        while (selectedProgram < '1' || selectedProgram > '5')
        {
            std::wcout << L"1: Rotes dreieck" << std::endl;
            std::wcout << L"2: Farbiges dreieck" << std::endl;
            std::wcout << L"3: Texturen" << std::endl;
            std::wcout << L"4: Rotierendes dreieck" << std::endl;
            std::wcout << L"5: Würfel" << std::endl;
            std::wcout << L"0: Exit" << std::endl;
            selectedProgram = _getche();
            std::cout << std::endl;
            if (selectedProgram == '0')
                return 0;
        }

        // OpenGl Framework initialisieren
        int error = glfwInit();
        if (error != GL_TRUE)
        {
            std::cerr << "Error loading GLFW" << std::endl;
            return -1;
        }
        // Neues Fenster erzeugen
        GLFWwindow* window = glfwCreateWindow(600, 600, "OpenGL", NULL, NULL);
        // Fenster als aktuellen Rendercontext festlegen
        glfwMakeContextCurrent(window);
        // Ausgabe Framerate auf die Frequenz des Monitors setzen
        glfwSwapInterval(1);

        // OpenGl Version in der Konsole ausgeben
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

        // Start-Zeitpunkt des Programms abspeichern
        auto startTime = std::chrono::steady_clock::now();

        // Textur laden
        int texture = LoadTexture();

        // Dauerschleife solange laufen lassen, bis das Fenster geschlossen wird
        while (!glfwWindowShouldClose(window))
        {
            // Zeit (in Sekunden) seit Programmstart ausrechnen
            float time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count() / 1000.0;

            // Fenster- und Eingabeereignisse abrufen
            glfwPollEvents();
            
            glfwMakeContextCurrent(window);
            // Aktuelle Größe des Fensters abrufen
            int windowW, windowH;
            glfwGetWindowSize(window, &windowW, &windowH);
            // Größe der Ausgabe anpassen
            glViewport(0, 0, windowW, windowH);

            switch (selectedProgram)
            {
            case '1':
                RedTriangle();
                break;
            case '2':
                ColoredTriangle();
                break;
            case '3':
                TexturedQuad(texture);
                break;
            case '4':
                RotatedTriangle(time);
                break;
            case '5':
                Cube(time, windowW, windowH);
                break;
            }

            // Aktuelles Bild auf dem Bildschirm ausgeben
            glfwSwapBuffers(window);
        }

        // Fenster schließen
        glfwDestroyWindow(window);

        // GLFW wieder entladen
        glfwTerminate();
    }
}
#pragma once

namespace Shader::Utils
{
bool almostEqual (float a, float b, float eps = 1e-6f);

int createProgram (const char *vertexShader, const char *geometryShader, const char *fragmentShader, const char *programName = nullptr);
}

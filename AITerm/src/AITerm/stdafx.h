// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#define INTERROGATIVE_PRONOUN		"./TextFiles/Interrogative_pronoun.txt"

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#include <stdio.h>
#include <tchar.h>
#include <memory.h>
#include <string.h>
#include <string>
#include <vector>

#include "TestCase.h"
#include "DBConnector.h"

#include "SimpleMinipar.h"
#include "MiniparUsage.h"
#include "NamedEntityRecognition.h"
#include "FileProcess.h"
#include "wn.h"
#include "XMLLib.h"


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
// Lab Xv6 and Unix utilities
// primes.c

#include "kernel/types.h"
#include "user/user.h"
#include "stddef.h"

// �ļ��������ض���(����ӳ��ȽϺ�)
void
mapping(int n, int pd[])
{
  // �ر��ļ������� n
  close(n);
  // ���ܵ��� ����д �˿ڸ��Ƶ������� n ��
  // ������һ�� n �� pd[n] ��ӳ��
  dup(pd[n]);
  // �رչܵ��е�������
  close(pd[0]);
  close(pd[1]);
}

// ������
void
primes()
{
  // ���������ȡ�ܵ��е���
  int previous, next;
  // ����ܵ�����������
  int fd[2];
  // �ӹܵ���ȡ����
  if (read(0, &previous, sizeof(int)))
  {
    // ��һ��һ����������ֱ�Ӵ�ӡ
    printf("prime %d\n", previous);
    // �����ܵ�
    pipe(fd);
    // �����ӽ���
    if (fork() == 0)
    {
      // �ӽ���
      // �ӽ��̽��ܵ���д�˿�ӳ�䵽������ 1 ��
      mapping(1, fd);
      // ѭ����ȡ�ܵ��е�����
      while (read(0, &next, sizeof(int)))
      {
        // ����������ǹܵ��е�һ�����ı���
        if (next % previous != 0)
        {
          // д��ܵ�
          write(1, &next, sizeof(int));
        }
      }
    }
    else
    {
      // ������
      // �ȴ��ӽ��̰�����ȫ��д��ܵ�
      wait(NULL);
      // �����̽��ܵ��Ķ��˿�ӳ�䵽������ 0 ��
      mapping(0, fd);
      // �ݹ�ִ�д˹���
      primes();
    }  
  }  
}

int 
main(int argc, char *argv[])
{
  // ����������
  int fd[2];
  // �����ܵ�
  pipe(fd);
  // ��������
  if (fork() == 0)
  {
    // �ӽ���
    // �ӽ��̽��ܵ���д�˿�ӳ�䵽������ 1 ��
    mapping(1, fd);
    // ѭ����ȡ 2 �� 35
    for (int i = 2; i < 36; i++)
    {
      // ����д��ܵ�
      write(1, &i, sizeof(int));
    }
  }
  else
  {
    // ������
    // �ȴ��ӽ��̰�����ȫ��д��ܵ�
    wait(NULL);
    // �����̽��ܵ��Ķ��˿�ӳ�䵽������ 0 ��
    mapping(0, fd);
    // ���� primes() ����������
    primes();
  }
  // �����˳�
  exit(0);
}


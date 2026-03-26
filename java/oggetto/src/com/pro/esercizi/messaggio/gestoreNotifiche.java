package com.pro.esercizi.messaggio;

import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

public class gestoreNotifiche {

  protected ArrayList<messaggio> notifiche = new ArrayList<>();

  public void aggiungiMessaggio(messaggio m) {
    if (m instanceof email) {
      if (m.testo.length() < 5000) {
        notifiche.add(m);

      }
      if (m instanceof sms) {
        if (m.testo.length() < 160) {
          notifiche.add(m);
        }
      }
    }
  }

  public void ArchiviaMessaggi(String nomeFile) {
    try {
      FileWriter fw = new FileWriter(nomeFile);
      fw.write("email");
      fw.write("sms");
      fw.close();
    } catch (IOException e) {
      System.out.println(e.getMessage());
    }
  }

  public void leggiArchivio(String nomeFile) {
    try {
      FileReader fr = new FileReader(nomeFile);
      int i;
      while ((i = fr.read()) != -1) {
        System.out.println((char) i);
      }
      fr.close();
    } catch (IOException e) {
      System.out.println(e.getMessage());
    }
  }

  public void inviaTutti() {
    for (messaggio m : notifiche) {
      m.invia();
    }
  }

}

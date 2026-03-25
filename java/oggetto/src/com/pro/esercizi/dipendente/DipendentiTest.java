import java.io.*;
import java.util.ArrayList;
import java.util.Locale;

public class DipendentiTest {
  private static final String FILE_DIPENDENTI = "dipendenti.txt";
  private static final String FILE_STIPENDI = "stipendi.txt";

  public static void main(String[] args) {
    ArrayList<Dipendente> dipendenti = new ArrayList<>();

    dipendenti.add(new Impiegato("Mario", "Rossi", 2000));
    dipendenti.add(new Impiegato("Anna", "Bianchi", 2200));
    dipendenti.add(new Impiegato("Giovanni", "Verdi", 2100));
    dipendenti.add(new Manager("Carlo", "Neri", 3000, 5000));
    dipendenti.add(new Manager("Laura", "Ferrari", 3200, 6000));
    dipendenti.add(new Impiegato("Paolo", "Gallo", 1900));

    salvaDipendenti(dipendenti);

    ArrayList<Dipendente> dipendentiLetti = leggiDipendenti();

    generaFileStipendi(dipendentiLetti);

  }

  private static void salvaDipendenti(ArrayList<Dipendente> dipendenti) {
    try (PrintWriter writer = new PrintWriter(new FileWriter(FILE_DIPENDENTI))) {
      for (Dipendente d : dipendenti) {
        if (d instanceof Manager) {
          Manager m = (Manager) d;
          writer.println(String.format(Locale.US, "M;%s;%s;%.2f;%.2f",
              m.getNome(), m.getCognome(), m.getStipendioBaseMensile(), m.getBonusAnnuo()));
        } else if (d instanceof Impiegato) {
          writer.println(String.format(Locale.US, "I;%s;%s;%.2f",
              d.getNome(), d.getCognome(), d.getStipendioBaseMensile()));
        }
      }
    } catch (IOException e) {
    }
  }

  private static ArrayList<Dipendente> leggiDipendenti() {
    ArrayList<Dipendente> dipendenti = new ArrayList<>();
    try (BufferedReader reader = new BufferedReader(new FileReader(FILE_DIPENDENTI))) {
      String linea;
      while ((linea = reader.readLine()) != null) {
        String[] parti = linea.split(";");
        if (parti.length > 0) {
          String tipo = parti[0];
          String nome = parti[1];
          String cognome = parti[2];
          double stipendioBaseMensile = Double.parseDouble(parti[3]);

          if (tipo.equals("I")) {
            dipendenti.add(new Impiegato(nome, cognome, stipendioBaseMensile));
          } else if (tipo.equals("M")) {
            double bonusAnnuo = Double.parseDouble(parti[4]);
            dipendenti.add(new Manager(nome, cognome, stipendioBaseMensile, bonusAnnuo));
          }
        }
      }
    } catch (IOException e) {

      return null;
    }
    return dipendenti;
  }

  private static void generaFileStipendi(ArrayList<Dipendente> dipendenti) {

    try (PrintWriter writer = new PrintWriter(new FileWriter(FILE_STIPENDI))) {
      double totaleStipendi = 0;

      writer.println("=== RIEPILOGO STIPENDI ANNUI ===\n");
      writer.println(String.format("%-15s %-15s %-15s %15s", "Nome", "Cognome", "Tipo", "Stipendio Annuo"));
      writer.println("=" + "=".repeat(58));

      for (Dipendente d : dipendenti) {
        String tipo = (d instanceof Manager) ? "Manager" : "Impiegato";
        double stipendioAnnuo = d.calcolaStipendioAnnuo();
        totaleStipendi += stipendioAnnuo;

        writer.println(String.format(Locale.US, "%-15s %-15s %-15s %15.2f",
            d.getNome(), d.getCognome(), tipo, stipendioAnnuo));
      }

      writer.println("=" + "=".repeat(58));
      writer.println(String.format(Locale.US, "%-47s %15.2f", "TOTALE STIPENDI ANNUI:", totaleStipendi));

    } catch (IOException e) {
    }
  }
}

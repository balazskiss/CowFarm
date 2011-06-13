using Gtk;
using WebKit;

public class ValaBrowser : Window {

    private const string TITLE = "CowFarm";
    private const string HOME_URL = "http://balazskiss.name/projects/CowFarm/CowFarm-Client/www";

    private WebView web_view;

    public ValaBrowser () {
        this.title = ValaBrowser.TITLE;
        set_default_size (900, 600);

        this.web_view = new WebView ();
        add(web_view);
        connect_signals ();
    }


    private void connect_signals () {
        this.destroy.connect (Gtk.main_quit);
    }

    public void start () {
        show_all ();
        this.web_view.open (ValaBrowser.HOME_URL);
    }

    public static int main (string[] args) {
        Gtk.init (ref args);

        var browser = new ValaBrowser ();
        browser.start ();

        Gtk.main ();

        return 0;
    }
}


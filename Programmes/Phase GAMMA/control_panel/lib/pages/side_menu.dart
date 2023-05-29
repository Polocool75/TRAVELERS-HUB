import 'package:control_panel/pages/auth.dart';
import 'package:control_panel/pages/home.dart';
import 'package:flutter/material.dart';
import 'package:url_launcher/url_launcher.dart';

class SideMenu extends StatelessWidget {
  const SideMenu({super.key});

  @override
  Widget build(BuildContext context) {
    return Drawer(
        child: ListView(
      padding: EdgeInsets.zero,
      children: [
        UserAccountsDrawerHeader(
          accountName: const Text('admin'),
          accountEmail: const Text('Travelers@gmail.com'),
          currentAccountPicture: CircleAvatar(
            child: ClipOval(
              child: Image.asset(
                'assets/img/logo1.jpg',
                fit: BoxFit.cover,
                width: 90,
                height: 90,
              ),
            ),
          ),
          decoration: const BoxDecoration(
            color: Color(0xFF1331F5),
          ),
        ),
        ListTile(
          leading: const Icon(Icons.home),
          title: const Text('Accueil'),
          onTap: () {
            MaterialPageRoute(builder: (context) => const HomePage());
          },
          trailing: ClipOval(
            child: Container(
              color: Colors.red,
              width: 20,
              height: 20,
              child: const Center(
                  child: Text(
                '1',
                style: TextStyle(color: Colors.white, fontSize: 12),
              )),
            ),
          ),
        ),
        ListTile(
          leading: const Icon(Icons.person),
          title: const Text('À propos'),
          onTap: () {
            launchUrl(Uri.parse('https://www.davincibot.fr'));
          },
        ),
        ListTile(
          leading: const Icon(Icons.contacts),
          title: const Text('Contact'),
          onTap: () {
            Navigator.pop(context);
          },
        ),
        const Divider(),
        ListTile(
          leading: const Icon(Icons.settings),
          title: const Text('Réglages'),
          onTap: () {
            Navigator.pop(context);
          },
        ),
        ListTile(
          leading: const Icon(Icons.logout),
          title: const Text('Déconnexion'),
          onTap: () {
            Navigator.pushReplacement(
              context,
              MaterialPageRoute(builder: (context) => LoginPage()),
            );
          },
        ),
      ],
    ));
  }
}

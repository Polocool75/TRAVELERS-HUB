import 'package:flutter/material.dart';

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
                'img/logo1.jpg',
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
          title: const Text('Home'),
          onTap: () {
            Navigator.pop(context);
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
          title: const Text('About'),
          onTap: () {
            Navigator.pop(context);
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
          title: const Text('Settings'),
          onTap: () {
            Navigator.pop(context);
          },
        ),
      ],
    ));
  }
}

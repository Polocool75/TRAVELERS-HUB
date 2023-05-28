import 'package:control_panel/pages/auth.dart';
import 'package:flutter/material.dart';
import 'package:control_panel/pages/side_menu.dart';

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {

  void onLaunch() {
    Navigator.push(
      context,
      MaterialPageRoute(builder: (context) =>  LoginPage())
      //pas encreo fini, doit avoir un autre page pour lancer le programme python
    );
  }
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Panneau de contrôle'),
        backgroundColor: const Color(0xFF1331F5),
      ),
      drawer: const SideMenu(),
      body:  SafeArea(
          child: Center(
        child: Column(
          children: [
            const SizedBox(height: 50),
            const Text('Bienvenue dans votre espace',
                style: TextStyle(fontSize: 20)),
            const Divider(),
            const SizedBox(height: 50),
            CustomButton(
              onTap: () => onLaunch(), 
              text: 'Exécuter programme python'),
          ],
        ),
      )),
    );
  }
}

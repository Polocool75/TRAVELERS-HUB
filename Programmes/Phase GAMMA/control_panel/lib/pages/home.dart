import 'package:flutter/material.dart';
import 'package:control_panel/pages/side_menu.dart';

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  String currentText = 'Activer caméra';
  bool isStreaming = false;
  String streamFolder = 'assets/lib/vid/';

  void toggleStreaming() {
    if (isStreaming) {
      setState(() {
        isStreaming = false;
        currentText = 'Activer caméra';
      });
    } else {
      setState(() {
        isStreaming = true;
        currentText = 'Désactiver caméra';
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Panneau de contrôle'),
        backgroundColor: const Color(0xFF1331F5),
      ),
      drawer: const SideMenu(),
      body: SafeArea(
        child: Center(
          child: Column(
            children: [
              const SizedBox(height: 20),
              const Text(
                'Bienvenue dans votre espace',
                style: TextStyle(fontSize: 20),
              ),
              const Divider(),
              const SizedBox(height: 30),
              if (isStreaming)
                SizedBox(
                  width: 640,
                  height: 480,
                  child: Expanded(
                    child: Center(
                      // PARTIE À CHANGER : afficher les vidéos consécutivement
                      child: Image.asset('img/vid1.jpg'),
                      // Balise <video> à utiliser pour afficher la vidéo
                    ),
                  ),
                ),
              if (!isStreaming)
                SizedBox(
                  width: 640,
                  height: 480,
                  child: Image.asset(
                      'img/no_img.jpg'), // Use default image when not streaming
                ),
              const SizedBox(height: 50),
              CustomButton(onTap: () => toggleStreaming(), text: currentText),
            ],
          ),
        ),
      ),
    );
  }
}

class CustomButton extends StatelessWidget {
  final VoidCallback onTap;
  final String text;

  const CustomButton({super.key, required this.onTap, required this.text});

  @override
  Widget build(BuildContext context) {
    return Container(
      padding: const EdgeInsets.symmetric(horizontal: 500),
      child: ElevatedButton(
        onPressed: onTap,
        style: ElevatedButton.styleFrom(
          backgroundColor: const Color(0xFF1331F5),
          padding: const EdgeInsets.symmetric(horizontal: 50, vertical: 20),
          textStyle: const TextStyle(
            fontSize: 20,
            fontWeight: FontWeight.bold,
          ),
        ),
        child: Text(text),
      ),
    );
  }
}

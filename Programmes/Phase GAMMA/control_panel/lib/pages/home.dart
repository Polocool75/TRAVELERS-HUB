import 'package:control_panel/pages/side_menu.dart';
import 'package:flutter/material.dart';
import 'package:control_panel/components/custom_button.dart';


class HomePage extends StatefulWidget {
  const HomePage({Key? key}) : super(key: key);

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  String currentText = 'Activer caméra';
  bool isStreaming = false;


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
                const SizedBox(
                  width: 640,
                  height: 480,
                ),
              if (!isStreaming)
                SizedBox(
                  width: 640,
                  height: 480,
                  child: Image.asset('img/no_img.jpg'),
                ),
              const SizedBox(height: 50),
              CustomButton(onTap: toggleStreaming, text: currentText),
            ],
          ),
        ),
      ),
    );
  }
}

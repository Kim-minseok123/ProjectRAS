# [Unreal Engine 5] Dungeon Crawler Portfolio 
## 목차
  - [소개](#소개) 
  - [개발 동기](#개발-동기)
  - [개발 환경](#개발-환경)
  - [사용 기술](#사용-기술)
  - [개발 과정](#개발-과정)
  - [플레이영상](#플레이영상)
  - [게임 다운로드](#게임-다운로드)
## 소개
<div align="center">

<img alt="1" src="https://github.com/user-attachments/assets/f5df7f3b-2bb0-456b-bf9b-ba456e70d64e" width="49%" height="230"/>
<img alt="2" src="https://github.com/user-attachments/assets/e1be16a2-d204-4410-8dec-10653c45369f" width="49%" height="230"/>
<img alt="3" src="https://github.com/user-attachments/assets/c725a7b2-177f-4bd3-9491-ed6271afdc02" width="49%" height="230"/>
<img alt="4" src="https://github.com/user-attachments/assets/6e1b3f4d-bd84-4594-998b-f7b5bd5a1c47" width="49%" height="230"/>
<img alt="5" src="https://github.com/user-attachments/assets/b9bc54a5-4403-41c8-b348-e80664157d07" width="49%" height="230"/>
<img alt="6" src="https://github.com/user-attachments/assets/7c2d0b24-82fa-46e5-9011-405e405ea843" width="49%" height="230"/>
<img alt="7" src="https://github.com/user-attachments/assets/6c83553f-bdbf-4889-8fa6-147f01fd1b38" width="49%" height="230"/>
<img alt="8" src="https://github.com/user-attachments/assets/d90b1245-bb14-4ede-8615-c828066dee17" width="49%" height="230"/>
<img alt="9" src="https://github.com/user-attachments/assets/9eb50373-d618-4352-bcf4-009440ec0c2f" width="49%" height="230"/>
<img alt="10" src="https://github.com/user-attachments/assets/2a61f266-f117-42b2-aa27-1dcf131a70c5" width="49%" height="230"/>
<img alt="11" src="https://github.com/user-attachments/assets/78c3984a-30b4-478c-ab73-505737a2d9a9" width="49%" height="230"/>
<img alt="12" src="https://github.com/user-attachments/assets/05b5b3eb-d249-4323-94c8-3d1a6fcdc76c" width="49%" height="230"/>

  < 게임 플레이 사진 >

</div>

+ Unreal Engine 5 던전 크롤러 포트폴리오 입니다.

+ 게임 개발자로서 언리얼 엔진에 공부하고자 시작된 포트폴리오입니다.

+ 현재 일부 유로 에셋으로 인해 소스코드와 일부만 공개되어 있습니다.

+ 개발 기간: 2025.02.10 ~ 2024.07.29 ( 약 6개월 )

+ 개발 인원 : 1인

+ 형상 관리: Git SourceTree

<br>

## 개발 동기

최근 게임 업계에서 언리얼 엔진의 활용도가 점점 높아지고 있는 흐름에 맞춰,  
**언리얼 엔진 5를 활용한 C++ 기반 게임 개발 역량**을 쌓기 위해 이 프로젝트를 시작했다.

언리얼 엔진의 구조와 워크플로를 깊이 이해하는 것을 목표로, 전체 구현의 90% 이상을 C++로 작성하고  
필요한 부분에 한해 블루프린트를 곁들여 효율적으로 개발하는 방식을 연습했다.

장르로는 **던전 크롤러**를 선택하여, 전투 시스템을 포함한 핵심 게임 요소를 가능한 한 모두 구현하고자 했다.  
플레이어 이동, 몬스터 AI, 전투 판정, 패링, 맵 생성 등  
실제 게임 제작 과정에서 필수적으로 요구되는 시스템을 직접 설계하고 구현하며,  
언리얼 엔진에서의 C++ 프로그래밍과 블루프린트의 장단점을 함께 체득하는 것을 목표로 했다.

<br>

## 개발 환경
+ unreal Engine 5.4.4

+ Visual Studio 2022

<br>

## 🛠️ 사용 기술

### 🎥 카메라 & 애니메이션 전환
- **전투 모드와 일반 모드에 따른 카메라 및 애니메이션 전환 시스템 구현**  
  - 전투 모드일 때와 일반 모드일 때, 카메라 움직임과 캐릭터 애니메이션을 각기 다르게 설정해 자연스러운 전환을 구현

### 🥋 콤보 시스템
- **플레이어 콤보 컴포넌트 제작**  
  - 현재 상태에 따라 입력값(플레이어 키 입력)에 맞춰 콤보 어택을 실행하도록 구성  
  - `DataAsset`을 사용해 입력과 콤보 전이 관계를 저장, 해당 데이터만 수정하면 콤보 추가나 변경이 가능하도록 설계

### 🛡️ 패링 시스템
- **정확한 방어 판정에 따른 패링 구현**  
  - 적의 공격을 정확히 막으면 주변 적을 밀쳐내고, 타이밍이 빗나가면 스태미나를 소모하도록 구현

### 🗺️ 절차적 맵 생성
- **BFS 알고리즘 기반 절차적 맵 생성**  
  - 매 판마다 랜덤하게 맵과 경로를 생성하고, 이에 맞춰 미니맵과 월드맵을 자동 생성

### 🤖 몬스터 AI
- **다양한 몬스터 AI 구현**  
  - 일반 몬스터는 상황별 행동 패턴을 설정하고,  
  - 보스 AI는 점수제 로직(플레이어와의 거리, 공격 쿨타임, 체력 등을 종합 평가)과 비헤이비어 트리를 활용해 최적의 행동을 선택하도록 설계

### ⚔️ 전투 판정
- **LineTrace 기반 무기 공격 판정 구현**  
  - 플레이어, 몬스터 무기의 시작 지점과 끝 지점을 하나의 선으로 이어, 해당 선에 닿은 물체에 데미지를 주는 방식으로 판정
  - 공격 몽타주에서 NotifyState를 활용

### 📄 데이터 관리
- **Data Asset 기반 게임 데이터 관리**  
  - 체력, 공격력, 이동 속도 등 주요 능력치를 Data Asset으로 관리
  - 코드 수정 없이 Data Asset 값만 변경해 밸런스를 조정하거나 새로운 몬스터를 손쉽게 추가할 수 있도록 설계

## 개발 과정

+ 블로그 개발 과정
https://toward-mainobject.tistory.com/category/Unreal%20Engine%205/ProjectRAS

연습
  + [[UE5] ProjectRAS - 캐릭터 움직임과 애니메이션](https://toward-mainobject.tistory.com/151)
  + [[UE5] ProjectRAS - 점프와 타이머, 카메라 범위 제한](https://toward-mainobject.tistory.com/152)
  + [[UE5] ProjectRAS - 공격 애니메이션과 화살 발사](https://toward-mainobject.tistory.com/153)
  + [[UE5] ProjectRAS - 스킬 디자인 구성](https://toward-mainobject.tistory.com/154)
  + [[UE5] ProjectRAS - 스킬 디자인 구성2](https://toward-mainobject.tistory.com/155)
  + [[UE5] ProjectRAS - 스텟 구성하기](https://toward-mainobject.tistory.com/156)
  + [[UE5] ProjectRAS - 몬스터 AI 제작하기](https://toward-mainobject.tistory.com/157)

본작
  + [[UE5] ProjectRAS - 콤보 공격 과 Rolling](https://toward-mainobject.tistory.com/158)
  + [[UE5] ProjectRAS - 시점 고정과 일반 몬스터](https://toward-mainobject.tistory.com/159)
  + [[UE5] ProjectRAS - 전투 공격 판정 과 패링, 적 타겟팅 시스템(락온)](https://toward-mainobject.tistory.com/160)
  + [[UE5] ProjectRAS - 패링으로 적 밀격 하기, UI 만들기](https://toward-mainobject.tistory.com/161)
  + [[UE5] ProjectRAS - 스테미나 추가, 패링 성공 시 Blur, 캐릭터 죽음](https://toward-mainobject.tistory.com/162)
  + [[UE5] ProjectRAS - 맵 디자인 및 블프화](https://toward-mainobject.tistory.com/163)
  + [[UE5] ProjectRAS - Map Generator](https://toward-mainobject.tistory.com/164)
  + [[UE5] ProjectRAS - 보스 패턴 제작하기](https://toward-mainobject.tistory.com/165)
  + [[UE5] ProjectRAS - UI 스킬 쿨타임 과 포션 마시기](https://toward-mainobject.tistory.com/166)
  + [[UE5] ProjectRAS - 몬스터 스폰하기, Clothes 적용하기](https://toward-mainobject.tistory.com/167)
  + [[UE5] ProjectRAS - 메인 Title UI와 메뉴 UI 만들기](https://toward-mainobject.tistory.com/168)
  + [[UE5] ProjectRAS - 미니맵, 월드맵 만들기](https://toward-mainobject.tistory.com/169)
  + [[UE5] ProjectRAS - Npc 만들기](https://toward-mainobject.tistory.com/170)
<br>

## 플레이영상
https://youtu.be/gt7QqVNnY90

## 게임 다운로드
https://drive.google.com/file/d/1AxTOIpDtvCT0OZgcXggSW6w8towLR21f/view?usp=sharing

